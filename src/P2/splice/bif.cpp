#include <splice/bif.h>
#include <splice/ref.h>
#include <splice/frame.h>
#include <po.h>
#include <util.h>
#include <gcc/math.h>

/**
 * @brief Global uniform float operation (UFO) dispatch table.
 * These are functions that operate on and return a single float value.
 */
PFNUFO s_mpufokpfnufo[UFOK_Max] = {
    /* 0 */ sqrtf,
    /* 1 */ sinf,
    /* 2 */ cosf,
    /* 3 */ tanf,
    /* 4 */ asinf,
    /* 5 */ acosf,
    /* 6 */ atanf,
    /* 7 */ RadNormalize,
};

/**
 * @brief Global BIF dispatch table.  This matches the ordering in BIFK
 * and defines the function dispatch, the number of required arguments, and
 * if the function takes variadic arguments.
 */
BIF g_mpbifkbif[BIFK_Max] = {
    /*   0 */ {RefOpAdd, 1, true},
    /*   1 */ {RefOpSub, 1, true},
    /*   2 */ {RefOpMult, 1, true},
    /*   3 */ {RefOpDiv, 1, true},
    /*   4 */ {RefOpPrint, 1, false},
    /*   5 */ {RefOpPrintFrame, 0, true},
    /*   6 */ {RefOpPrintSidebag, 1, false},
    /*   7 */ {RefOpIntEqual, 2, false},
    /*   8 */ {RefOpL, 2, false},
    /*   9 */ {RefOpLE, 2, false},
    /*  10 */ {RefOpG, 2, false},
    /*  11 */ {RefOpGE, 2, false},
    /*  12 */ {RefOpEqv, 2, false},
    /*  13 */ {RefOpEqual, 2, false},
    /*  14 */ {RefOpIsBoolean, 1, false},
    /*  15 */ {RefOpIsNum, 1, false},
    /*  16 */ {RefOpIsInteger, 1, false},
    /*  17 */ {RefOpIsFloat, 1, false},
    /*  18 */ {RefOpIsSymbol, 1, false},
    /*  19 */ {RefOpIsVector, 1, false},
    /*  20 */ {RefOpIsMatrix, 1, false},
    /*  21 */ {RefOpIsClq, 1, false},
    /*  22 */ {RefOpIsLm, 1, false},
    /*  23 */ {RefOpIsSmp, 1, false},
    /*  24 */ {RefOpIsList, 1, false},
    /*  25 */ {RefOpIsNull, 1, false},
    /*  26 */ {RefOpIsObject, 1, false},
    /*  27 */ {RefOpIsNullObj, 1, false},
    /*  28 */ {RefOpIsMethod, 1, false},
    /*  29 */ {RefOpIsProcedure, 1, false},
    /*  30 */ {RefOpAreNear, 3, false},
    /*  31 */ {RefOpNot, 1, false},
    /*  32 */ {RefOpCons, 2, false},
    /*  33 */ {RefOpCar, 1, false},
    /*  34 */ {RefOpCdr, 1, false},
    /*  35 */ {RefOpSetCar, 2, false},
    /*  36 */ {RefOpSetCdr, 2, false},
    /*  37 */ {RefOpLength, 1, false},
    /*  38 */ {RefOpNth, 2, false},
    /*  39 */ {RefOpIsMember, 2, false},
    /*  40 */ {RefOpList, 0, true},
    /*  41 */ {RefOpAppend, 0, true},
    /*  42 */ {RefOpMap, 2, false},
    /*  43 */ {RefOpFilter, 2, false},
    /*  44 */ {RefOpForEach, 2, false},
    /*  45 */ {RefOpEval, 1, false},
    /*  46 */ {RefOpVector, 3, false},
    /*  47 */ {RefOpMatrix, 0, true},
    /*  48 */ {RefOpClq, 3, false},
    /*  49 */ {RefOpLm, 2, false},
    /*  50 */ {RefOpSmp, 3, false},
    /*  51 */ {RefOpGetElement, 1, true},
    /*  52 */ {RefOpRandomSeed, 1, false},
    /*  53 */ {RefOpRandom, 2, false},
    /*  54 */ {RefOpSqrt, 1, false},
    /*  55 */ {RefOpSin, 1, false},
    /*  56 */ {RefOpCos, 1, false},
    /*  57 */ {RefOpTan, 1, false},
    /*  58 */ {RefOpAsin, 1, false},
    /*  59 */ {RefOpAcos, 1, false},
    /*  60 */ {RefOpAtan, 1, false},
    /*  61 */ {RefOpRadNormalize, 1, false},
    /*  62 */ {RefOpAtan2, 2, false},
    /*  63 */ {RefOpVectorDotProduct, 2, false},
    /*  64 */ {RefOpVectorCrossProduct, 2, false},
    /*  65 */ {RefOpVectorLth, 1, false},
    /*  66 */ {RefOpVectorDistance, 1, true},
    /*  67 */ {RefOpVectorDistanceSquared, 1, true},
    /*  68 */ {RefOpVectorNormalize, 1, true},
    /*  69 */ {RefOpVectorProjectNormal, 2, false},
    /*  70 */ {RefOpVectorProjectTangent, 2, false},
    /*  71 */ {RefOpVectorBallisticVelocity, 3, true},
    /*  72 */ {RefOpVectorRadianNormal, 2, false},
    /*  73 */ {RefOpMatrixTranspose, 1, false},
    /*  74 */ {RefOpMatrixInvert, 1, false},
    /*  75 */ {RefOpMatrixCalculateDmat, 2, false},
    /*  76 */ {RefOpMatrixInterpolateRotate, 3, false},
    /*  77 */ {RefOpMatrixDecomposeToTranslate, 1, false},
    /*  78 */ {RefOpMatrixDecomposeToRotate, 1, false},
    /*  79 */ {RefOpMatrixDecomposeToEuler, 1, false},
    /*  80 */ {RefOpMatrixDecomposeToRadianNormal, 1, false},
    /*  81 */ {RefOpMatrixLookAt, 2, true},
    /*  82 */ {RefOpMatrixTiltUpright, 1, false},
    /*  83 */ {RefOpClqEvaluate, 2, false},
    /*  84 */ {RefOpClqEvaluateLm, 3, false},
    /*  85 */ {RefOpClqFit, 4, false},
    /*  86 */ {RefOpLmLimit, 2, false},
    /*  87 */ {RefOpLmCheck, 2, false},
    /*  88 */ {RefOpFloor, 1, false},
    /*  89 */ {RefOpCeiling, 1, false},
    /*  90 */ {RefOpRound, 1, false},
    /*  91 */ {RefOpTruncate, 1, false},
    /*  92 */ {RefOpAbs, 1, false},
    /*  93 */ {RefOpMaximum, 1, true},
    /*  94 */ {RefOpMinimum, 1, true},
    /*  95 */ {RefOpModulo, 2, false},
    /*  96 */ {RefOpCurrentTime, 0, false},
    /*  97 */ {RefOpScheduleCallback, 2, true},
    /*  98 */ {RefOpDeferObjectUpdate, 2, false},
    /*  99 */ {RefOpAddO, 3, false},
    /* 100 */ {RefOpEnsureO, 3, false},
    /* 101 */ {RefOpSetO, 3, false},
    /* 102 */ {RefOpGetO, 2, false},
    /* 103 */ {RefOpFindObject, 2, false},
    /* 104 */ {RefOpFindObjects, 2, false},
    /* 105 */ {RefOpFindNearestObject, 2, false},
    /* 106 */ {RefOpFindNearestObjects, 2, false},
    /* 107 */ {RefOpFindPlayerObject, 0, false},
    /* 108 */ {RefOpFindWorldObject, 0, false},
    /* 109 */ {RefOpFindCameraObject, 0, false},
    /* 110 */ {RefOpFindClassObjects, 2, false},
    /* 111 */ {RefOpFindObjectsInBoundingBox, 2, false},
    /* 112 */ {RefOpFindObjectsInBoundingSphere, 2, false},
    /* 113 */ {RefOpHitTestObjects, 3, false},
    /* 114 */ {RefOpHitTestObjectsFirst, 3, false},
    /* 115 */ {RefOpConvertObjectPosition, 3, false},
    /* 116 */ {RefOpConvertObjectVector, 3, false},
    /* 117 */ {RefOpConvertObjectMatrix, 3, false},
    /* 118 */ {RefOpNearClipCenter, 0, false},
    /* 119 */ {RefOpStartSound, 2, false},
    /* 120 */ {RefOpStopSound, 1, false},
    /* 121 */ {RefOpStartRumble, 2, false},
    /* 122 */ {RefOpEmitSmokeCloud, 2, false},
    /* 123 */ {RefOpPredictAnimationEffect, 7, false},
    /* 124 */ {RefOpSetMusicRegister, 2, false},
};

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpAdd__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpSub__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMult__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpDiv__FiP4CRefP6CFrame);

CRef RefOpPrint(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetTag(TAGK_Void);
    return CRef(cref);
}

CRef RefOpPrintFrame(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetTag(TAGK_Void);
    return CRef(cref);
}

CRef RefOpPrintSidebag(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetTag(TAGK_Void);
    return CRef(cref);
}

CRef RefOpIntEqual(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref[0].m_tag.m_n == aref[1].m_tag.m_n);
    return CRef(cref);
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefCmp__FP4CRef4CMPK);

CRef RefOpL(int carg, CRef *aref, CFrame *pframe)
{
    return RefCmp(aref, CMPK_L);
}

CRef RefOpLE(int carg, CRef *aref, CFrame *pframe)
{
    return RefCmp(aref, CMPK_LE);
}

CRef RefOpG(int carg, CRef *aref, CFrame *pframe)
{
    return RefCmp(aref, CMPK_G);
}

CRef RefOpGE(int carg, CRef *aref, CFrame *pframe)
{
    return RefCmp(aref, CMPK_GE);
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpEqv__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefEqualHelper__FP4CRefT0);

CRef RefOpEqual(int carg, CRef *aref, CFrame *pframe)
{
    return RefEqualHelper(&aref[0], &aref[1]);
}

CRef RefOpIsBoolean(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Bool);
    return CRef(cref);
}

CRef RefOpIsNum(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_S32 || aref->m_tagk == TAGK_F32);
    return CRef(cref);
}

CRef RefOpIsFloat(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_F32);
    return CRef(cref);
}

CRef RefOpIsInteger(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_S32);
    return CRef(cref);
}

CRef RefOpIsSymbol(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Symid);
    return CRef(cref);
}

CRef RefOpIsVector(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Vector);
    return CRef(cref);
}

CRef RefOpIsMatrix(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Matrix);
    return CRef(cref);
}

CRef RefOpIsClq(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Clq);
    return CRef(cref);
}

CRef RefOpIsLm(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Lm);
    return CRef(cref);
}

CRef RefOpIsSmp(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Smp);
    return CRef(cref);
}

CRef RefOpIsList(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Pair || aref->m_tagk == TAGK_None);
    return CRef(cref);
}

CRef RefOpIsNull(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_None);
    return CRef(cref);
}

CRef RefOpIsObject(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Basic);
    return CRef(cref);
}

CRef RefOpIsNullObj(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Basic && !aref->m_tag.m_pbasic);
    return CRef(cref);
}

CRef RefOpIsMethod(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Method);
    return CRef(cref);
}

CRef RefOpIsProcedure(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Proc || aref->m_tagk == TAGK_Bifk);
    return CRef(cref);
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpAreNear__FiP4CRefP6CFrame);

CRef RefOpNot(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBool(aref->m_tagk == TAGK_Bool && !aref->m_tag.m_bool);
    return CRef(cref);
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpCons__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpCar__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpCdr__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpSetCadr__FiP4CRefP6CFrame4BIFK);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpSetCar__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpSetCdr__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpLength__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpNth__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpIsMember__FiP4CRefP6CFrame);

CRef RefOpList(int carg, CRef *aref, CFrame *pframe)
{
    return CRef(*aref);
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpAppend__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMap__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFilter__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpForEach__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpEval__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVector__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrix__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpSetMusicRegister__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpClq__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpLm__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpSmp__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpGetElement__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpRandomSeed__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpRandom__FiP4CRefP6CFrame);

CRef RefUfo(CRef *pref, UFOK ufok)
{
    float fval;
    CRef ref;

    if (pref->m_tagk == TAGK_F32)
    {
        fval = pref->m_tag.m_g;
    }
    else
    {
        fval = (float)pref->m_tag.m_n;
    }

    ref.SetF32(s_mpufokpfnufo[ufok](fval));
    return ref;
}

CRef RefOpSqrt(int carg, CRef *aref, CFrame *pframe)
{
    return RefUfo(aref, UFOK_Sqrt);
}

CRef RefOpSin(int carg, CRef *aref, CFrame *pframe)
{
    return RefUfo(aref, UFOK_Sin);
}

CRef RefOpCos(int carg, CRef *aref, CFrame *pframe)
{
    return RefUfo(aref, UFOK_Cos);
}

CRef RefOpTan(int carg, CRef *aref, CFrame *pframe)
{
    return RefUfo(aref, UFOK_Tan);
}

CRef RefOpAsin(int carg, CRef *aref, CFrame *pframe)
{
    return RefUfo(aref, UFOK_Asin);
}

CRef RefOpAcos(int carg, CRef *aref, CFrame *pframe)
{
    return RefUfo(aref, UFOK_Acos);
}

CRef RefOpAtan(int carg, CRef *aref, CFrame *pframe)
{
    return RefUfo(aref, UFOK_Atan);
}

CRef RefOpRadNormalize(int carg, CRef *aref, CFrame *pframe)
{
    return RefUfo(aref, UFOK_RadNormalize);
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpAtan2__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorDotProduct__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorCrossProduct__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorLth__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorDistance__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorDistanceSquared__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorNormalize__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorProjectNormal__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorProjectTangent__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorBallisticVelocity__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpVectorRadianNormal__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixTranspose__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixInvert__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixCalculateDmat__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixInterpolateRotate__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixDecomposeToTranslate__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixDecomposeToRotate__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixDecomposeToEuler__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixLookAt__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMatrixTiltUpright__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpClqEvaluate__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpClqEvaluateLm__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpClqFit__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpLmLimit__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpLmCheck__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFloor__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpCeiling__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpRound__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpTruncate__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpAbs__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMaximum__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMinimum__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpModulo__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpCurrentTime__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpScheduleCallback__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpDeferObjectUpdate__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpObjectOption__FiP4CRefP6CFrame4BIFK);

CRef RefOpAddO(int carg, CRef *aref, CFrame *pframe)
{
    return RefOpObjectOption(carg, aref, pframe, BIFK_AddO);
}

CRef RefOpEnsureO(int carg, CRef *aref, CFrame *pframe)
{
    return RefOpObjectOption(carg, aref, pframe, BIFK_EnsureO);
}

CRef RefOpSetO(int carg, CRef *aref, CFrame *pframe)
{
    return RefOpObjectOption(carg, aref, pframe, BIFK_SetO);
}

CRef RefOpGetO(int carg, CRef *aref, CFrame *pframe)
{
    return RefOpObjectOption(carg, aref, pframe, BIFK_GetO);
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefPairFromAplo__FiPP2LO);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFindObject__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFindObjects__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFindNearestObject__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFindNearestObjects__FiP4CRefP6CFrame);

CRef RefOpFindPlayerObject(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBasic(PpoCur());
    return CRef(cref);
}

CRef RefOpFindWorldObject(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBasic(g_psw);
    return CRef(cref);
}

CRef RefOpFindCameraObject(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();
    cref.SetBasic(g_pcm);
    return CRef(cref);
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFindClassObjects__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFindObjectsInBoundingBox__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpFindObjectsInBoundingSphere__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpHitTestObjects__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpHitTestObjectsFirst__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpConvertObjectPosition__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpConvertObjectVector__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpConvertObjectMatrix__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpNearClipCenter__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpStartSound__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpStopSound__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpStartRumble__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpEmitSmokeCloud__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpPredictAnimationEffect__FiP4CRefP6CFrame);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", __8VU_FLOATf);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", __9VU_VECTORRC6VECTOR);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", __as__6VECTORG9VU_VECTOR);

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", __ml__FG8VU_FLOATG9VU_VECTOR);
