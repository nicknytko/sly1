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

CRef *CFrame::PrefFindBinding(SYMID symid, int fRecursive)
{
    /* Set this flag to true while we are searching for bindings in case there is a loop
     * in the frame DAG. */
    fSearchingBinding = true;
    CRef *pFound = NULL;

    /* First search for bindings in this frame */
    for (CBinding *pbinding = m_pbindingHead; pbinding != NULL; pbinding = pbinding->m_pbindingNext)
    {
        if (pbinding->m_symid == symid)
        {
            pFound = &pbinding->m_ref;
            goto done;
        }
    }

    /* If we didn't find the binding, recurse on the parent frames */
    if (fRecursive)
    {
        for (int ipframe = 0; ipframe < m_cpframeParent; ipframe++)
        {
            CFrame *pframeParent = m_apframeParent[ipframe];
            if (!pframeParent->fSearchingBinding)
            {
                pFound = pframeParent->PFoundFindBinding(symid, fRecursive);
                if (pFound != NULL)
                {
                    goto done;
                }
            }
        }
    }

done:
    fSearchingBinding = false;
    return pFound;
}

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
