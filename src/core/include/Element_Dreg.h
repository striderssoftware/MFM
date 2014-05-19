#ifndef ELEMENT_DREG_H  /* -*- C++ -*- */
#define ELEMENT_DREG_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "P1Atom.h"
#include "Element_Res.h"  /* For Element_Res::TYPE */
#include "Element_Wall.h" /* For Element_Wall::TYPE */

namespace MFM
{

#define DREG_RES_ODDS 100
#define DREG_DEL_ODDS 40
#define DREG_DRG_ODDS 200
#define DREG_DDR_ODDS 20 /*Deleting DREGs*/

#define DREG_VERSION 1

  template <class CC>
  class Element_Dreg : public Element<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

  public:

    static Element_Dreg THE_INSTANCE;
    static const u32 TYPE() {
      return THE_INSTANCE.GetType();
    }

    Element_Dreg() : Element<CC>(MFM_UUID_FOR("Dreg", DREG_VERSION)) { }

    virtual const T & GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);
      return defaultAtom;
    }

    virtual u32 PercentMovable(const T& you,
			       const T& me, const SPoint& offset) const
    {
      return 100;
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xff505050;
    }

    virtual void Behavior(EventWindow<CC>& window) const
    {
      Random & random = window.GetRandom();

      SPoint dir;
      MDist<R>::get().FillRandomSingleDir(dir, random);

      if (window.IsLiveSite(dir)) {

        T atom = window.GetRelativeAtom(dir);
        u32 oldType = atom.GetType();

        if(Element_Empty<CC>::IsType(oldType))
          {
            if(random.OneIn(DREG_DRG_ODDS))
              {
                atom = Element_Dreg<CC>::THE_INSTANCE.GetDefaultAtom();
              }
            else if(random.OneIn(DREG_RES_ODDS))
              {
                atom = Element_Res<CC>::THE_INSTANCE.GetDefaultAtom();
              }
          }
        else if(oldType == Element_Dreg::TYPE())
          {
            if(random.OneIn(DREG_DDR_ODDS))
              {
                atom = T();
              }
          }
        else if(oldType != Element_Wall<CC>::TYPE() && random.OneIn(DREG_DEL_ODDS))
          {
            atom = T();
          }

        if(atom.GetType() != oldType)
          {
            window.SetRelativeAtom(dir, atom);
          }
      }
      this->Diffuse(window);

    }
  };

  template <class CC>
  Element_Dreg<CC> Element_Dreg<CC>::THE_INSTANCE;

}

#endif /* ELEMENT_DREG_H */