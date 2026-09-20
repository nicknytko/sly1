#include <splice/binding.h>
#include <splice/splotheap.h>
#include <sce/memset.h>

inline void *operator new(size_t, void *place)
{
    return place;
}

void CBinding::CloneTo(CBinding *pbindingClone, CFrame *pframeClone)
{
    pbindingClone->m_symid = m_symid;
    m_ref.CloneTo(&pbindingClone->m_ref, pframeClone);

    if (m_pbindingNext)
    {
        CBinding *pbindingNew = PbindingNew();
        m_pbindingNext->CloneTo(pbindingNew, pframeClone);
        pbindingClone->m_pbindingNext = pbindingNew;
    }
}

CBinding *PbindingNew()
{
    CBinding *pbinding = (CBinding *)g_splotheapUnk1.PvAllocClear();
    memset(pbinding, 0, sizeof(CBinding));
    new (pbinding) CBinding();
    return pbinding;
}

void DeleteBinding(CBinding *pbinding)
{
    pbinding->m_ref.~CRef();
}
