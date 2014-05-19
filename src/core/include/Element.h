#ifndef ELEMENT_H /* -*- C++ -*- */
#define ELEMENT_H

#include "itype.h"
#include "StaticLoader.h"
#include "EventWindow.h"
#include "UUID.h"
#include "Dirs.h"
#include "Logger.h"

namespace MFM
{
  typedef u32 ElementType;

  template <class C> class Atom; // Forward declaration

  template <class CC>
  class Element
  {
  private:
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

    const UUID m_UUID;
    const u32 m_type;

  protected:
    static const SPoint VNNeighbors[4];

    bool FillAvailableVNNeighbor(EventWindow<CC>& window, SPoint& pt) const;

    bool FillPointWithType(EventWindow<CC>& window,
			   SPoint& pt, const SPoint* relevants, u32 relevantCount,
			   Dir rotation, ElementType type) const;

    void Diffuse(EventWindow<CC>& window) const;


    /**
     * Determines how likely an Atom of this type is to be swapped
     * with during diffusal.
     *
     * @returns an integral percentage, from 0 to 100, describing the
     *          desire of an atom of this type to be moved; 0 being
     *          impossible to move and 100 being completely fine with
     *          moving.
     */
    virtual u32 PercentMovable(const T& you,
			       const T& me, const SPoint& offset) const = 0;

  public:

    Element(const UUID & uuid) : m_UUID(uuid), m_type(U16StaticLoader::AllocateType(m_UUID))
    {
      LOG.Message("%0x for %@",m_type,&m_UUID);
    }

    // For use by Element_Empty only!
    Element(const UUID & uuid, u32 type) : m_UUID(uuid), m_type(type)
    { }

    //    u32 GetType() const { return GetDefaultAtom().GetType(); }
    u32 GetType() const { return m_type; }

    bool IsType(u32 type) const { return m_type == type; }

    const UUID & GetUUID() const {
      return m_UUID;
    }

    virtual void Behavior(EventWindow<CC>& window) const = 0;

    virtual const T & GetDefaultAtom() const = 0;

    virtual u32 DefaultPhysicsColor() const = 0;

    virtual u32 LocalPhysicsColor(const T &, u32 selector) const {
      return DefaultPhysicsColor();
    }

    /**
     * On entry, the Atom at \a nowAt will be an instance of the type
     * of this Element.  How much does that atom like the idea that it
     * should be moved to (or remain at; the two SPoints might be
     * equal) location \a maybeAt?  Return 0 to flat-out veto the
     * idea, or from 0 up to COMPLETE_DIFFUSABILITY to request that
     * fraction of diffusability.  (Values larger than
     * COMPLETE_DIFFUSABILITY are treated as equal to it.)  Note that
     * \a nowAt and \a maybeAt are both relative to the EventWindow \a
     * ew, but neither is necessarily the center atom!
     *
     * By default all atoms are considered diffusable, and they return
     * COMPLETE_DIFFUSABILITY chances for every suggested move
     */
    virtual u32 Diffusability(EventWindow<CC> & ew, SPoint nowAt, SPoint maybeAt) const {
      return COMPLETE_DIFFUSABILITY;
    }

    /**
       A utility method available to subclasses that specifies no
       diffusion is allowed, by specifying a 0 value for the
       Diffusability of every position except when \a nowAt == \a
       maybeAt.
     */
    u32 NoDiffusability(EventWindow<CC> & ew, SPoint nowAt, SPoint maybeAt) const {
      return (nowAt == maybeAt)? COMPLETE_DIFFUSABILITY : 0;
    }

    /**
     * A standard basis for specifying degrees of diffusability.
     */
    static const u32 COMPLETE_DIFFUSABILITY = 1000;

  };
}

#include "Element.tcc"

#endif /* ELEMENT_H */