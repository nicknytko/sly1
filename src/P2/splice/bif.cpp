#include <splice/bif.h>
#include <splice/ref.h>
#include <splice/frame.h>
#include <splice/pair.h>
#include <splice/vecmat.h>

#include <gcc/math.h>
#include <util.h>
#include <po.h>
#include <sce/rand.h>

/**
 * @brief Function pointer for uniform float operation dispatch.
 */
typedef float (*PFNUFO)(float);

/**
 * @brief Table of uniform float operation functions.
 */
static PFNUFO s_mpufokpfnufo[] =
{
    sqrtf,        // UFOK_Sqrt
    sinf,         // UFOK_Sin
    cosf,         // UFOK_Cos
    tanf,         // UFOK_Tan
    asinf,        // UFOK_Asin
    acosf,        // UFOK_Acos
    atanf,        // UFOK_Atan
    RadNormalize, // UFOK_RadNormalize
};

/**
 * @brief Table of built-in functions.
 */
BIF g_mpbifkbif[125] =
{
    {RefOpAdd, 1, 1},
    {RefOpSub, 1, 1},
    {RefOpMult, 1, 1},
    {RefOpDiv, 1, 1},
    {RefOpPrint, 1, 0},
    {RefOpPrintFrame, 0, 1},
    {RefOpPrintSidebag, 1, 0},
    {RefOpIntEqual, 2, 0},
    {RefOpL, 2, 0},
    {RefOpLE, 2, 0},
    {RefOpG, 2, 0},
    {RefOpGE, 2, 0},
    {RefOpEqv, 2, 0},
    {RefOpEqual, 2, 0},
    {RefOpIsBoolean, 1, 0},
    {RefOpIsNum, 1, 0},
    {RefOpIsInteger, 1, 0},
    {RefOpIsFloat, 1, 0},
    {RefOpIsSymbol, 1, 0},
    {RefOpIsVector, 1, 0},
    {RefOpIsMatrix, 1, 0},
    {RefOpIsClq, 1, 0},
    {RefOpIsLm, 1, 0},
    {RefOpIsSmp, 1, 0},
    {RefOpIsList, 1, 0},
    {RefOpIsNull, 1, 0},
    {RefOpIsObject, 1, 0},
    {RefOpIsNullObj, 1, 0},
    {RefOpIsMethod, 1, 0},
    {RefOpIsProcedure, 1, 0},
    {RefOpAreNear, 3, 0},
    {RefOpNot, 1, 0},
    {RefOpCons, 2, 0},
    {RefOpCar, 1, 0},
    {RefOpCdr, 1, 0},
    {RefOpSetCar, 2, 0},
    {RefOpSetCdr, 2, 0},
    {RefOpLength, 1, 0},
    {RefOpNth, 2, 0},
    {RefOpIsMember, 2, 0},
    {RefOpList, 0, 1},
    {RefOpAppend, 0, 1},
    {RefOpMap, 2, 0},
    {RefOpFilter, 2, 0},
    {RefOpForEach, 2, 0},
    {RefOpEval, 1, 0},
    {RefOpVector, 3, 0},
    {RefOpMatrix, 0, 1},
    {RefOpClq, 3, 0},
    {RefOpLm, 2, 0},
    {RefOpSmp, 3, 0},
    {RefOpGetElement, 1, 1},
    {RefOpRandomSeed, 1, 0},
    {RefOpRandom, 2, 0},
    {RefOpSqrt, 1, 0},
    {RefOpSin, 1, 0},
    {RefOpCos, 1, 0},
    {RefOpTan, 1, 0},
    {RefOpAsin, 1, 0},
    {RefOpAcos, 1, 0},
    {RefOpAtan, 1, 0},
    {RefOpRadNormalize, 1, 0},
    {RefOpAtan2, 2, 0},
    {RefOpVectorDotProduct, 2, 0},
    {RefOpVectorCrossProduct, 2, 0},
    {RefOpVectorLth, 1, 0},
    {RefOpVectorDistance, 1, 1},
    {RefOpVectorDistanceSquared, 1, 1},
    {RefOpVectorNormalize, 1, 1},
    {RefOpVectorProjectNormal, 2, 0},
    {RefOpVectorProjectTangent, 2, 0},
    {RefOpVectorBallisticVelocity, 3, 1},
    {RefOpVectorRadianNormal, 2, 0},
    {RefOpMatrixTranspose, 1, 0},
    {RefOpMatrixInvert, 1, 0},
    {RefOpMatrixCalculateDmat, 2, 0},
    {RefOpMatrixInterpolateRotate, 3, 0},
    {RefOpMatrixDecomposeToTranslate, 1, 0},
    {RefOpMatrixDecomposeToRotate, 1, 0},
    {RefOpMatrixDecomposeToEuler, 1, 0},
    {RefOpMatrixDecomposeToRadianNormal, 1, 0},
    {RefOpMatrixLookAt, 2, 1},
    {RefOpMatrixTiltUpright, 1, 0},
    {RefOpClqEvaluate, 2, 0},
    {RefOpClqEvaluateLm, 3, 0},
    {RefOpClqFit, 4, 0},
    {RefOpLmLimit, 2, 0},
    {RefOpLmCheck, 2, 0},
    {RefOpFloor, 1, 0},
    {RefOpCeiling, 1, 0},
    {RefOpRound, 1, 0},
    {RefOpTruncate, 1, 0},
    {RefOpAbs, 1, 0},
    {RefOpMaximum, 1, 1},
    {RefOpMinimum, 1, 1},
    {RefOpModulo, 2, 0},
    {RefOpCurrentTime, 0, 0},
    {RefOpScheduleCallback, 2, 1},
    {RefOpDeferObjectUpdate, 2, 0},
    {RefOpAddO, 3, 0},
    {RefOpEnsureO, 3, 0},
    {RefOpSetO, 3, 0},
    {RefOpGetO, 2, 0},
    {RefOpFindObject, 2, 0},
    {RefOpFindObjects, 2, 0},
    {RefOpFindNearestObject, 2, 0},
    {RefOpFindNearestObjects, 2, 0},
    {RefOpFindPlayerObject, 0, 0},
    {RefOpFindWorldObject, 0, 0},
    {RefOpFindCameraObject, 0, 0},
    {RefOpFindClassObjects, 2, 0},
    {RefOpFindObjectsInBoundingBox, 2, 0},
    {RefOpFindObjectsInBoundingSphere, 2, 0},
    {RefOpHitTestObjects, 3, 0},
    {RefOpHitTestObjectsFirst, 3, 0},
    {RefOpConvertObjectPosition, 3, 0},
    {RefOpConvertObjectVector, 3, 0},
    {RefOpConvertObjectMatrix, 3, 0},
    {RefOpNearClipCenter, 0, 0},
    {RefOpStartSound, 2, 0},
    {RefOpStopSound, 1, 0},
    {RefOpStartRumble, 2, 0},
    {RefOpEmitSmokeCloud, 2, 0},
    {RefOpPredictAnimationEffect, 7, 0},
    {RefOpSetMusicRegister, 2, 0},
};

CRef RefOpAdd(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref;

    TAGK tagk = aref[0].m_tagk;
    CPair *pCurrent;
    if (aref[1].m_tagk == TAGK_None)
    {
        pCurrent = NULL;
    }
    else
    {
        pCurrent = aref[1].GetPair();
    }

    switch (tagk)
    {
        case TAGK_S32:
        case TAGK_F32:
        {
            float acc;
            int fIsInt;

            if (tagk == TAGK_S32)
            {
                acc = aref[0].GetS32();
                fIsInt = 1;
            }
            else
            {
                acc = aref[0].GetF32();
                fIsInt = 0;
            }

            while (pCurrent != NULL)
            {
                if (pCurrent->m_ref.m_tagk == TAGK_S32)
                {
                    acc += pCurrent->m_ref.GetS32();
                }
                else
                {
                    fIsInt = 0;
                    acc += pCurrent->m_ref.GetF32();
                }
                pCurrent = pCurrent->m_ppairNext;
            }

            if (fIsInt)
            {
                cref.SetS32((int) acc);
            }
            else
            {
                cref.SetF32(acc);
            }
            break;
        }
        case TAGK_Vector:
        {
            VECTOR *pVector = PvectorNew();
            VECTOR *pVectorSrc = aref->GetVector();
            pVector->data = pVectorSrc->data;

            while (pCurrent != NULL)
            {
                /* Loads the vectors into vf1 and vf2, adds them together, then reads the result */
                __asm__ __volatile__(
                    "lqc2      $vf1, 0x0(%0)\n"
                    "lqc2      $vf2, 0x0(%1)\n"
                    "vadd.xyzw $vf1, $vf1, $vf2\n"
                    "sqc2      $vf1, 0x0(%0)\n"
                    : : "r"(pVector), "r"(pCurrent->m_ref.GetVector()) : "memory"
                );
                pCurrent = pCurrent->m_ppairNext;
            }

            cref.SetVector(pVector);
            break;
        }
        case TAGK_Matrix:
        {
            MATRIX4 *pMatrix = PmatrixNew();
            MATRIX4 *pMatrixSrc = aref->GetMatrix();

            pMatrix->data[0] = pMatrixSrc->data[0];
            pMatrix->data[1] = pMatrixSrc->data[1];
            pMatrix->data[2] = pMatrixSrc->data[2];
            pMatrix->data[3] = pMatrixSrc->data[3];

            while (pCurrent != NULL)
            {
                MATRIX4 *pMatrixOther = pCurrent->m_ref.GetMatrix();
                pMatrix->ag[ 0] += pMatrixOther->ag[ 0];
                pMatrix->ag[ 1] += pMatrixOther->ag[ 1];
                pMatrix->ag[ 2] += pMatrixOther->ag[ 2];
                pMatrix->ag[ 3] += pMatrixOther->ag[ 3];
                pMatrix->ag[ 4] += pMatrixOther->ag[ 4];
                pMatrix->ag[ 5] += pMatrixOther->ag[ 5];
                pMatrix->ag[ 6] += pMatrixOther->ag[ 6];
                pMatrix->ag[ 7] += pMatrixOther->ag[ 7];
                pMatrix->ag[ 8] += pMatrixOther->ag[ 8];
                pMatrix->ag[ 9] += pMatrixOther->ag[ 9];
                pMatrix->ag[10] += pMatrixOther->ag[10];
                pMatrix->ag[11] += pMatrixOther->ag[11];
                pMatrix->ag[12] += pMatrixOther->ag[12];
                pMatrix->ag[13] += pMatrixOther->ag[13];
                pMatrix->ag[14] += pMatrixOther->ag[14];
                pMatrix->ag[15] += pMatrixOther->ag[15];

                pCurrent = pCurrent->m_ppairNext;
            }

            cref.SetMatrix(pMatrix);
            break;
        }
    }

    return cref;
}

CRef RefOpSub(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref;

    TAGK tagk = aref[0].m_tagk;
    CPair *pCurrent;
    if (aref[1].m_tagk == TAGK_None)
    {
        pCurrent = NULL;
    }
    else
    {
        pCurrent = aref[1].GetPair();
    }

    switch (tagk)
    {
        case TAGK_S32:
        case TAGK_F32:
        {
            float acc;
            int fIsInt;

            if (tagk == TAGK_S32)
            {
                acc = aref[0].GetS32();
                fIsInt = 1;
            }
            else
            {
                acc = aref[0].GetF32();
                fIsInt = 0;
            }

            while (pCurrent != NULL)
            {
                if (pCurrent->m_ref.m_tagk == TAGK_S32)
                {
                    acc -= pCurrent->m_ref.GetS32();
                }
                else
                {
                    fIsInt = 0;
                    acc -= pCurrent->m_ref.GetF32();
                }
                pCurrent = pCurrent->m_ppairNext;
            }

            if (fIsInt)
            {
                cref.SetS32((int) acc);
            }
            else
            {
                cref.SetF32(acc);
            }
            break;
        }
        case TAGK_Vector:
        {
            VECTOR *pVector = PvectorNew();
            VECTOR *pVectorSrc = aref->GetVector();
            pVector->data = pVectorSrc->data;

            while (pCurrent != NULL)
            {
                /* Loads the vectors into vf1 and vf2, subtracts them, then reads the result */
                __asm__ __volatile__(
                    "lqc2      $vf1, 0x0(%0)\n"
                    "lqc2      $vf2, 0x0(%1)\n"
                    "vsub.xyzw $vf1, $vf1, $vf2\n"
                    "sqc2      $vf1, 0x0(%0)\n"
                    : : "r"(pVector), "r"(pCurrent->m_ref.GetVector()) : "memory"
                );
                pCurrent = pCurrent->m_ppairNext;
            }

            cref.SetVector(pVector);
            break;
        }
        case TAGK_Matrix:
        {
            MATRIX4 *pMatrix = PmatrixNew();
            MATRIX4 *pMatrixSrc = aref->GetMatrix();

            pMatrix->data[0] = pMatrixSrc->data[0];
            pMatrix->data[1] = pMatrixSrc->data[1];
            pMatrix->data[2] = pMatrixSrc->data[2];
            pMatrix->data[3] = pMatrixSrc->data[3];

            while (pCurrent != NULL)
            {
                MATRIX4 *pMatrixOther = pCurrent->m_ref.GetMatrix();
                pMatrix->ag[ 0] -= pMatrixOther->ag[ 0];
                pMatrix->ag[ 1] -= pMatrixOther->ag[ 1];
                pMatrix->ag[ 2] -= pMatrixOther->ag[ 2];
                pMatrix->ag[ 3] -= pMatrixOther->ag[ 3];
                pMatrix->ag[ 4] -= pMatrixOther->ag[ 4];
                pMatrix->ag[ 5] -= pMatrixOther->ag[ 5];
                pMatrix->ag[ 6] -= pMatrixOther->ag[ 6];
                pMatrix->ag[ 7] -= pMatrixOther->ag[ 7];
                pMatrix->ag[ 8] -= pMatrixOther->ag[ 8];
                pMatrix->ag[ 9] -= pMatrixOther->ag[ 9];
                pMatrix->ag[10] -= pMatrixOther->ag[10];
                pMatrix->ag[11] -= pMatrixOther->ag[11];
                pMatrix->ag[12] -= pMatrixOther->ag[12];
                pMatrix->ag[13] -= pMatrixOther->ag[13];
                pMatrix->ag[14] -= pMatrixOther->ag[14];
                pMatrix->ag[15] -= pMatrixOther->ag[15];

                pCurrent = pCurrent->m_ppairNext;
            }

            cref.SetMatrix(pMatrix);
            break;
        }
    }

    return cref;
}

INCLUDE_ASM("asm/nonmatchings/P2/splice/bif", RefOpMult__FiP4CRefP6CFrame);
#ifdef SKIP_ASM
CRef RefOpMult(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref = CRef();

    /* The possible values that can represent the product */
    CPair* ppair;
    MATRIX4* pmatrix = NULL;
    LM* plm = NULL;
    VECTOR* pvector = NULL;
    CLQ* pclq = NULL;
    float gscalar = 0.0;

    /* The current type of the product */
    TAGK tagProductType;
    int fisInt = 0;

    /* Determine the initial value that gets stored */
    switch (aref[0].m_tagk)
    {
        case TAGK_F32:
        {
            tagProductType = TAGK_F32;
            gscalar = aref->GetF32();
            fisInt = false;
            break;
        }
        case TAGK_S32:
        {
            tagProductType = TAGK_F32;
            gscalar = (float) aref->GetS32();
            fisInt = true;
            break;
        }
        case TAGK_Vector:
        {
            tagProductType = TAGK_Vector;
            pvector = PvectorNew();
            *pvector = *aref->GetVector();
        }
        case TAGK_Matrix:
        {
            tagProductType = TAGK_Matrix;
            pmatrix = PmatrixNew();

            MATRIX4* pmatrixSrc = aref->GetMatrix();
            pmatrix->data[0] = pmatrixSrc->data[0];
            pmatrix->data[1] = pmatrixSrc->data[1];
            pmatrix->data[2] = pmatrixSrc->data[2];
            pmatrix->data[3] = pmatrixSrc->data[3];
        }
        case TAGK_Clq:
        {
            tagProductType = TAGK_Clq;
            pclq = PclqNew();
            *pclq = *aref->GetClq();
        }
        case TAGK_Lm:
        {
            tagProductType = TAGK_Lm;
            plm = PlmNew();
            *plm = *aref->GetLm();
        }
    }

    ppair = NULL;
    if (aref[1].m_tagk != TAGK_None)
    {
        ppair = aref[1].GetPair();
    }

    /* Now traverse the pair and dispatch multiplication based on the current accumulator type and the new type.
       Not every type multiplies with each other, and incompatible types will be silently ignored. */
    while (ppair != NULL)
    {
        switch (tagProductType)
        {
            case TAGK_F32:
            {
                /* Scalar dispatch, will automatically "up-cast" to higher order types */
                switch (ppair->m_ref.m_tagk)
                {
                    case TAGK_S32:
                    {
                        gscalar = gscalar * (float) ppair->m_ref.GetS32();
                        break;
                    }
                    case TAGK_F32:
                    {
                        fisInt = false;
                        gscalar = gscalar * ppair->m_ref.GetF32();
                        break;
                    }
                    case TAGK_Vector:
                    {
                        tagProductType = TAGK_Vector;
                        pvector = PvectorNew();
                        /* scalar vector multiplication */
                        break;
                    }
                    case TAGK_Matrix:
                    {
                        tagProductType = TAGK_Matrix;
                        pmatrix = PmatrixNew();
                        MATRIX4 ret = (*ppair->m_ref.GetMatrix()) * VU_FLOAT(gscalar);
                        MATRIX4 temp;
                        memcpy(&temp, &ret, sizeof(MATRIX4));
                        *pmatrix = temp;
                        break;
                    }
                    case TAGK_Clq:
                    {
                        tagProductType = TAGK_Clq;
                        pclq = PclqNew();
                        *(VECTOR*) pclq = VU_FLOAT(gscalar) * VU_VECTOR(*(VECTOR*) ppair->m_ref.GetClq());
                        break;
                    }
                    case TAGK_Lm:
                    {
                        tagProductType = TAGK_Lm;
                        plm = PlmNew();
                        plm->gMin = ppair->m_ref.GetLm()->gMin * gscalar;
                        plm->gMax = ppair->m_ref.GetLm()->gMax * gscalar;
                        break;
                    }
                }
                break;
            }
            case TAGK_Vector:
            {
                TAGK tagOperand = ppair->m_ref.m_tagk;
                if (tagOperand == TAGK_S32)
                {
                    /* scalar vector multiplication */
                }
                else if (tagOperand == TAGK_F32)
                {
                    /* scalar vector multiplication */
                }
                break;
            }
            case TAGK_Matrix:
            {
                TAGK tagOperand = ppair->m_ref.m_tagk;
                switch (tagOperand)
                {
                    case TAGK_F32:
                    case TAGK_S32:
                    {
                        VU_FLOAT vf(tagOperand == TAGK_F32 ?
                                    ppair->m_ref.GetF32() :
                                    (float) ppair->m_ref.GetS32());

                        MATRIX4 ret = (*pmatrix) * vf;
                        MATRIX4 temp;
                        memcpy(&temp, &ret, sizeof(MATRIX4));
                        *pmatrix = temp;
                        break;
                    }
                    case TAGK_Vector:
                    {
                        pvector = PvectorNew();
                        MultiplyMatrix4Vector(pmatrix, ppair->m_ref.GetVector(), 1.0f,  pvector);
                        tagProductType = TAGK_Vector;
                        break;
                    }
                    case TAGK_Matrix:
                    {
                        *pmatrix = (*pmatrix) * (*ppair->m_ref.GetMatrix());
                        break;
                    }
                }
                break;
            }
        }

        ppair = ppair->m_ppairNext;
    }

    /* Now place the result in the output reference */
    switch (tagProductType)
    {
        case TAGK_F32:
        {
            if (fisInt)
            {
                cref.SetS32((int) gscalar);
            }
            else
            {
                cref.SetF32(gscalar);
            }
            break;
        }
        case TAGK_Vector:
        {
            cref.SetVector(pvector);
            if (pmatrix)
            {
                DeleteMatrix(pmatrix);
            }
            break;
        }
        case TAGK_Matrix:
        {
            cref.SetMatrix(pmatrix);
            break;
        }
        case TAGK_Clq:
        {
            cref.SetClq(pclq);
            break;
        }
        case TAGK_Lm:
        {
            cref.SetLm(plm);
            break;
        }
    }

    return cref;
}
#endif

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
    cref.SetBool(aref[0].GetS32() == aref[1].GetS32());
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
    cref.SetBool(aref->m_tagk == TAGK_Basic && !aref->GetBasic());
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
    cref.SetBool(aref->m_tagk == TAGK_Bool && !aref->GetBool());
    return CRef(cref);
}

CRef RefOpCons(int carg, CRef* aref, CFrame *pframe)
{
    CRef cref;

    /* Construct a new pair whose first entry is aref[0] */
    CPair *ppair = PpairNew();
    ppair->m_ref = *aref;

    /* Now the remainder pointer depends on the type of aref[1] */
    CRef *pcdr = aref + 1;
    if (pcdr->m_tagk == TAGK_None)
    {
        ppair->m_ppairNext = NULL;
    }
    else if (pcdr->m_tagk == TAGK_Pair)
    {
        ppair->m_ppairNext = pcdr->GetPair();
    }

    cref.SetPair(ppair);
    return cref;
}

CRef RefOpCar(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref;
    cref = aref->GetPair()->m_ref;
    return cref;
}

CRef RefOpCdr(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref;
    CPair *ppair = aref->GetPair();

    if (ppair->m_ppairNext != NULL)
    {
        cref.SetPair(ppair->m_ppairNext);
    }
    else
    {
        cref.SetTag(TAGK_None);
    }

    return cref;
}

CRef RefOpSetCadr(int carg, CRef *aref, CFrame *pframe, BIFK bifk)
{
    CRef cref;
    CPair *ppair = aref[0].GetPair();

    if (bifk == BIFK_SetCar)
    {
        ppair->m_ref = aref[1];
    }
    else if (aref[1].m_tagk == TAGK_Pair)
    {
        ppair->m_ppairNext = aref[1].GetPair();
    }
    else
    {
        ppair->m_ppairNext = NULL;
    }

    cref.SetTag(TAGK_Void);
    return cref;
}

CRef RefOpSetCar(int carg, CRef *aref, CFrame *pframe)
{
    return RefOpSetCadr(carg, aref, pframe, BIFK_SetCar);
}

CRef RefOpSetCdr(int carg, CRef *aref, CFrame *pframe)
{
    return RefOpSetCadr(carg, aref, pframe, BIFK_SetCdr);
}

CRef RefOpLength(int carg, CRef *aref, CFrame *pframe)
{
    CRef ref;

    int nlength = 0;
    if (aref->m_tagk == TAGK_Pair)
    {
        CPair *ppair = aref->GetPair();
        while (ppair != NULL)
        {
            ppair = ppair->m_ppairNext;
            nlength++;
        }
    }

    ref.SetS32(nlength);
    return ref;
}

CRef RefOpNth(int carg, CRef *aref, CFrame *pframe)
{
    CRef ref;
    ref.SetTag(TAGK_None);

    if (aref->m_tagk == TAGK_Pair)
    {
        CRef *pref = aref + 1;
        int cur = 0;
        for (CPair *ppair = aref->GetPair(); ppair != NULL; ppair = ppair->m_ppairNext)
        {
            if (cur == pref->GetS32())
            {
                ref = ppair->m_ref;
                break;
            }

            cur++;
        }
    }

    return ref;
}

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

CRef RefOpRandomSeed(int carg, CRef *aref, CFrame *pframe)
{
    CRef ref;

    if (aref->m_tagk == TAGK_S32)
    {
        srand(aref->GetS32());
    }
    else
    {
        srand((int) aref->GetF32());
    }

    ref.m_tagk = TAGK_Void;
    return ref;
}

CRef RefOpRandom(int carg, CRef *aref, CFrame *pframe)
{
    CRef ref;
    float gLow;
    float gHigh;

    /* Pull low part of range... */
    if (aref[0].m_tagk == TAGK_S32)
    {
        gLow = aref[0].GetS32();
    }
    else
    {
        gLow = aref[0].GetF32();
    }

    /* ... pull high part of range ... */
    if (aref[1].m_tagk == TAGK_S32)
    {
        gHigh = aref[1].GetS32();
    }
    else
    {
        gHigh = aref[1].GetF32();
    }

    /* ...now generate a value between them */
    ref.SetF32(GRandInRange(gLow, gHigh));

    return ref;
}

CRef RefUfo(CRef *pref, UFOK ufok)
{
    float fval;
    CRef ref;

    if (pref->m_tagk == TAGK_F32)
    {
        fval = pref->GetF32();
    }
    else
    {
        fval = (float)pref->GetS32();
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

CRef RefOpAtan2(int carg, CRef *aref, CFrame *pframe)
{
    CRef cref;

    float ag[2];
    float *pg = ag;

    for (int i = 0; i < 2; i++)
    {
        if (aref->m_tagk == TAGK_F32)
        {
            *pg++ = aref->GetF32();
        }
        else
        {
            *pg++ = aref->GetS32();
        }

        aref++;
    }

    cref.SetF32(atan2f(ag[0], ag[1]));

    return cref;
}

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

VU_FLOAT::VU_FLOAT(float g)
{
    /* This looks weird, but it seems this has to be an array to
       force a stack allocation */
    qword q[1];
    __asm__ __volatile__("mfc1 %0, %1" : "=r"(q[0]) : "f"(g));
    data = q[0];
}

VU_VECTOR::VU_VECTOR(const VECTOR& pvec)
{
    data = pvec.data;
}

VECTOR& VECTOR::operator=(VU_VECTOR pvec)
{
    data = pvec.data;
    return *this;
}

VU_VECTOR operator*(VU_FLOAT s, VU_VECTOR v)
{
    /* Load s and v into vf1 and vf2, respectively, multiply vf1.x * vf2,
       then copy it back to result */
    VU_VECTOR result = v;
    __asm__ __volatile__(
        "qmtc2.ni %1, $vf1\n"
        "qmtc2.ni %2, $vf2\n"
        "vmulx.xyzw $vf1, $vf2, $vf1x\n"
        "qmfc2.ni %0, $vf1"
        : "=r"(result.data)
        : "r"(s.data), "r"(v.data)
    );
    return result;
}
