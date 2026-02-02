#pragma once
#include "Sexy/RtWeakPtr.h"
#include "Reflection/RType.h"


#define OFFSET_REFLECTION_RCUSTOMTYPE_TSTDVECTORMANIPULATOR_FUNC2 0x5B2440
#define OFFSET_REFLECTION_RCUSTOMTYPE_TSTDVECTORMANIPULATOR_FUNC3 0x14F8794
using namespace Sexy;
namespace Reflection
{
	namespace RCustomType
	{
        struct UnkStruct
        {
        public:
            int unk1;
            int unk2;
            int unk3;
            int unk4;
            //RtWeakPtr<void>* weakPtrPtr;

            std::vector<long long> unkVec;
        };

        struct VectorManipulatorStruct
        {
        public:
            UnkStruct* unkPtr;
            void* unkPtr2;
            bool unkBool;
        };

        template <typename T>
        class TStdVectorManipulator
        {
        public:
            using GenericVector = std::vector<T>;

            virtual void NullSub() {};

            virtual void DestroyEffect(void* a1) {
                operator delete(a1);
            };

            bool UnkFunc(UnkStruct* a1, void* a2, int* vecSize) {
                //int* v1 = vecSize;
                *vecSize = 0;
                typedef int64_t(*func_2)(UnkStruct*, void*);
                func_2 pFunc2 = (func_2)getActualOffset(0x5B2AA4);

                uint v10 = pFunc2(a1, a2);

                typedef int64_t(*func_3)(UnkStruct*, int64_t);
                func_3 pFunc3 = (func_3)getActualOffset(0x14FEDCC);

                if (v10 && (pFunc3(a1, v10)) != 0) {
                    a1->unkVec.push_back(v10);
                    typedef int64_t(*func_4)(UnkStruct*, int64_t);
                    func_4 pFunc4 = (func_4)getActualOffset(0x14FEE04);
                    int v2 = pFunc4(a1, v10);
                    *vecSize = v2;
                }
                else
                {
                    return false;
                }
                return true;
            }
            virtual bool Edit(GenericVector* vector,
                VectorManipulatorStruct* manipInfo,
                RtWeakPtr<void>* unkWeakPtr,
                RType* reflectionType)
            {
                //typedef bool(*func_5)(TStdVectorManipulator<T>*, GenericVector*, VectorManipulatorStruct*, RtWeakPtr<void>*, RType*);
                //func_5 sub_14FEE04 = (func_5)getActualOffset(0x68BF08);
                //return sub_14FEE04(this, vector, manipInfo, unkWeakPtr, reflectionType);

                if (manipInfo->unkBool)
                {
                    int vecSize = 0;

                    if (TStdVectorManipulator::UnkFunc(manipInfo->unkPtr, unkWeakPtr, &vecSize))
                    {
                        if (vecSize > 0)
                        {
                            //LOGI("Begin manip vector (size: %d)", vecSize);
                            vector->resize(vecSize);

                            for (int i = 0; i < vecSize; i++)
                            {
                                T* element = &vector->at(i);
                                SexyString str;
                                reflectionType->Function11(element, manipInfo, str);
                            }
                        }

                        manipInfo->unkPtr->unkVec.pop_back();
                        return true;
                    }

                    return false;
                }
                else
                {
                    //void** v19 = *(void***)(manipInfo + 4);
                    void* v19 = manipInfo->unkPtr2;
                    typedef void (*sub262EC0)(void*, RtWeakPtr<void>*, int, char);
                    ((sub262EC0)getActualOffset(0x5B2440))(v19, unkWeakPtr, vector->size(), 0xFD);

                    //if (!vector->empty()) {
                    for (int i = 0; i < vector->size(); ++i) {
                        T* element = &vector->at(i);
                        SexyString str;
                        reflectionType->Function11(element, manipInfo, str);
                    }
                    //}

                    int64_t v11 = *(int64_t*)v19;
                    //void* v17 = v19;
                    //v19[2] = (int*)((char*)v19[2] - 1);
                    --*((int64_t*)v19 + 2);


                    uint8_t v21 = 254;
                    typedef void (*sub10D2100)(int64_t, uint8_t*, int);
                    ((sub10D2100)getActualOffset(0x14F8794))(v11, &v21, 1);

                    return true;

                    // LOGW("Panic! Need to edit vector but manipInfo->unkBool is false");
                }
            }
            virtual int GetSize(GenericVector* vector)
            {
                return vector->size();
            }
        };
	}
}