#include <splice/frame.h>
#include <splice/splotheap.h>
#include <splice/ref.h>
#include <splice/binding.h>
#include <sce/memset.h>

void CFrame::SetSingleParent(CFrame *pframeParent)
{
    m_apframeParent[0] = pframeParent;
    m_cpframeParent = 1;
}

void CFrame::AddParent(CFrame *pframeParent)
{
    m_apframeParent[m_cpframeParent++] = pframeParent;
}

CRef CFrame::RefAddBinding(SYMID symid, CRef *pref)
{
    CRef cref;
    CBinding* pbinding = PbindingNew();
    pbinding->m_symid = symid;
    pbinding->m_ref = *pref;
    pbinding->m_pbindingNext = m_pbindingHead;
    m_pbindingHead = pbinding;

    cref.SetTag(TAGK_Void);
    return cref;
}

CRef CFrame::RefSetBinding(SYMID symid, CRef *pref)
{
    CRef cref;

    /* Update the symbol if we find it */
    CRef* pBoundSymbol = PrefFindBinding(symid, true);
    if (pBoundSymbol != NULL) {
        *pBoundSymbol = *pref;
    }

    cref.SetTag(TAGK_Void);
    return cref;
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/frame", FFindBinding__6CFrameUiiP4CRef);

INCLUDE_ASM("asm/nonmatchings/P2/splice/frame", PrefFindBinding__6CFrameUii);

INCLUDE_ASM("asm/nonmatchings/P2/splice/frame", CloneTo__6CFrameP6CFrame);

CFrame *PframeNew()
{
    CFrame *pframe = (CFrame *)g_splotheapFrame.PvAllocClear();
    memset(pframe, 0, sizeof(CFrame));
    return pframe;
}

void DeleteFrame(CFrame *pframe)
{
    return;
}
