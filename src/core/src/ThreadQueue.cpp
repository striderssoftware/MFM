/* -*- C++ -*- */
#include <stdlib.h>
#include "ThreadQueue.h"
#include "Fail.h"
#include "Logger.h"

#define MIN(X,Y) ((X) > (Y) ? (Y) : (X))

namespace MFM
{
  ThreadQueue::ThreadQueue()
  {
    m_readHead = m_writeHead = m_heldBytes = 0;

    if(pthread_mutex_init(&m_lock, NULL))
    {
      LOG.Error("ThreadQueue Mutex did not initialize.\n");
      FAIL(OUT_OF_RESOURCES);
    }
    if(pthread_cond_init(&m_cond, NULL))
    {
      LOG.Error("ThreadQueue Cond did not initialize.\n");
      FAIL(OUT_OF_RESOURCES);
    }
  }

  ThreadQueue::~ThreadQueue()
  {
    pthread_mutex_destroy(&m_lock);
    pthread_cond_destroy(&m_cond);
  }

  void ThreadQueue::Write(u8* bytes, u32 length)
  {
    pthread_mutex_lock(&m_lock);
    {
      if(length + m_heldBytes > THREADQUEUE_MAX_BYTES)
      {
	LOG.Error("ERROR: THREADQUEUE OVERLOAD!\n");
	FAIL(OUT_OF_RESOURCES);
      }

      for(u32 i = 0; i < length; i++)
      {
	m_queueData[m_writeHead++] = *(bytes++);
	if(m_writeHead >= THREADQUEUE_MAX_BYTES)
	{
	  m_writeHead %= THREADQUEUE_MAX_BYTES;
	}
      }
      m_heldBytes += length;
    }
    pthread_cond_signal(&m_cond); /* Reading is available! */
    pthread_mutex_unlock(&m_lock);
  }

  u32 ThreadQueue::UnsafeRead(u8* bytes, u32 length)
  {
    u32 bytesAvailable = MIN(length, m_heldBytes);
    for(u32 i = 0; i < bytesAvailable; i++)
    {
      *(bytes++) = m_queueData[m_readHead++];
      if(m_readHead >= THREADQUEUE_MAX_BYTES)
      {
	m_readHead %= THREADQUEUE_MAX_BYTES;
      }
    }
    m_heldBytes -= bytesAvailable;
    return bytesAvailable;
  }

  void ThreadQueue::ReadBlocking(u8* bytes, u32 length)
  {
    u32 readBytes = 0;
    while(readBytes < length)
    {
      pthread_mutex_lock(&m_lock);
      {
	readBytes += UnsafeRead(bytes + readBytes, length);
	if(readBytes < length)
	{
	  pthread_cond_wait(&m_cond, &m_lock);
	}
      }
      pthread_mutex_unlock(&m_lock);
    }
  }

  u32 ThreadQueue::Read(u8* bytes, u32 length)
  {
    u32 readBytes = 0;
    /* Bail if there aren't enough bytes to read. */
    if(m_heldBytes >= length)
    {
      pthread_mutex_lock(&m_lock);
      {
	readBytes = UnsafeRead(bytes, length);
      }
      pthread_mutex_unlock(&m_lock);
    }
    return readBytes;
  }

  u32 ThreadQueue::BytesAvailable()
  {
    u32 ret;
    pthread_mutex_lock(&m_lock);
    ret = m_heldBytes;
    pthread_mutex_unlock(&m_lock);
    return ret;
  }

  void ThreadQueue::Flush()
  {
    pthread_mutex_lock(&m_lock);
    {
      m_readHead = m_writeHead = m_heldBytes = 0;
    }
    pthread_mutex_unlock(&m_lock);
  }
}
