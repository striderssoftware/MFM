/* -*- C++ -*- */
#include "UlamClass.h"
#include "UlamClassRegistry.h"

namespace MFM {

  template<class EC>
  void DebugPrint(const Tile<EC>& tile, const UlamRef<EC>& ur, ByteSink& out)
  {
    const u32 printFlags =
      UlamClassPrintFlags::PRINT_SYMBOL |
      UlamClassPrintFlags::PRINT_FULL_NAME |
      UlamClassPrintFlags::PRINT_ATOM_BODY |
      UlamClassPrintFlags::PRINT_MEMBER_NAMES |
      UlamClassPrintFlags::PRINT_MEMBER_VALUES |
      UlamClassPrintFlags::PRINT_INDENTED_LINES |
      UlamClassPrintFlags::PRINT_RECURSE_QUARKS;

    const UlamClassRegistry<EC> & ucr = tile.GetUlamClassRegistry();
    ur.Print(ucr, out, printFlags);
  }

  template<class EC>
  void DebugPrint(const Tile<EC>& tile, const typename EC::ATOM_CONFIG::ATOM_TYPE& atom, ByteSink& out)
  {
    u32 type = atom.GetType();
    bool sane = atom.IsSane();

    if (!sane) out.Printf("[insane]");

    const Element<EC>* elt = tile.GetElement(type);
    if (elt) 
    {
      const UlamElement<EC>* ue = elt->AsUlamElement();
      if (ue) 
      {
        UlamContext<EC> uc;
        AtomBitStorage<EC> abs(atom);
        UlamRef<EC> ur(EC::ATOM_CONFIG::ATOM_TYPE::ATOM_FIRST_STATE_BIT,
                       ue->GetClassLength(),
                       abs,
                       ue,
                       UlamRef<EC>::ATOMIC,
                       uc);
        DebugPrint(tile, ur, out);
        return;
      } 
    }

    typename EC::ATOM_CONFIG::ATOM_TYPE nonConstAtom(atom);
    AtomSerializer<typename EC::ATOM_CONFIG> as(nonConstAtom);
    if (elt)
      out.Printf("(%s) %s: ", elt->GetAtomicSymbol(), elt->GetName());

    out.Printf("[%04x] %@\n", type, &as);
  }

  template<class EC>
  void InitDebugTools()
  {
    UlamClassRegistry<EC> ucr;
    AtomBitStorage<EC> bs;
    UlamRef<EC> ur(0,0,bs,0);
    DebugPrint(ucr, ur, DevNullByteSink);
  }
}
