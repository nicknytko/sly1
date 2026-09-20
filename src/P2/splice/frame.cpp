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

int CFrame::FFindBinding(SYMID symid, int fRecursive, CRef *pref)
{
    CRef* pBoundSymbol = PrefFindBinding(symid, fRecursive);
    if (pBoundSymbol != NULL) {
        if (pref != NULL) {
            *pref = *pBoundSymbol;
        }
        return true;
    } else {
        return false;
    }
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/frame", PrefFindBinding__6CFrameUii);
#ifdef SKIP_ASM
CRef *CFrame::PrefFindBinding(SYMID symid, int fRecursive)
{
    fSearchingBinding = true;
    CBinding* pBindingCurrent = m_pbindingHead;
    CRef *pFound = NULL;

    /* Search through the current bindings of this frame for the symbol */
    while (pBindingCurrent != NULL)
    {
        if (pBindingCurrent->m_symid != symid)
        {
            pBindingCurrent = pBindingCurrent->m_pbindingNext;
        }
        else
        {
            pFound = &pBindingCurrent->m_ref;
        }
    }

    /* If we haven't found it and this is a recursive call, then also check
       the parent frames. */
    if (pBindingCurrent == NULL && !fRecursive)
    {
        int nParentFrameIdx = 0;
        if (m_cpframeParent > 0)
        {
            do
            {
                CFrame* pParentFrame = m_apframeParent[nParentFrameIdx];
                if (pParentFrame->fSearchingBinding == 0 ||
                    pParentFrame->PrefFindBinding(symid, fRecursive) != NULL)
                {
                    break;
                }
            } while (nParentFrameIdx < m_cpframeParent);
        }
    }

    fSearchingBinding = false;
    return pFound;
}
#endif

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
