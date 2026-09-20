/**
 * @file splice/binding.h
 */
#ifndef SPLICE_BINDING_H
#define SPLICE_BINDING_H

#include "common.h"
#include <splice/ref.h>

class CFrame;

/**
 * A splice symbol binding.  Ties a symbol id to a reference.
 */
struct CBinding
{
    int m_symid;
    CRef m_ref;
    CBinding *m_pbindingNext;

    CBinding()
    {
        m_ref.m_tagk = TAGK_Nil;
    }

    void CloneTo(CBinding *pbindingClone, CFrame *pframeClone);
};

CBinding *PbindingNew();
void DeleteBinding(CBinding *pbinding);

#endif // SPLICE_BINDING_H
