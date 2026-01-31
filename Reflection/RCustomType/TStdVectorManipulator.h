#pragma once
#include "Sexy/RtWeakPtr.h"
#include "Reflection/RType.h"


#define OFFSET_REFLECTION_RCUSTOMTYPE_TSTDVECTORMANIPULATOR_FUNC2 0x5B2440
#define OFFSET_REFLECTION_RCUSTOMTYPE_TSTDVECTORMANIPULATOR_FUNC3 0x14F8794

namespace Reflection
{
	namespace RCustomType
	{
        struct UnkStruct
        {
        public:
            int64_t unk1;
            int64_t unk2;
            int64_t unk3;
            int64_t unk4;
            Sexy::RtWeakPtr<void>* weakPtrPtr;
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

			virtual bool Edit(GenericVector* vector,VectorManipulatorStruct* manipInfo,Sexy::RtWeakPtr<void>* unkWeakPtr,RType* reflectionType)
			{
                if (manipInfo->unkBool)
                {
                    typedef void* (*Func5B2AA4)(UnkStruct*, Sexy::RtWeakPtr<void>*);
                    Func5B2AA4 resolveObj = (Func5B2AA4)getActualOffset(0x5B2AA4);
                    void* resolvedObject = resolveObj(manipInfo->unkPtr, unkWeakPtr);

                    typedef bool (*Func14FEDCC)(UnkStruct*, void*);
                    Func14FEDCC pFunc = (Func14FEDCC)getActualOffset(0x14FEDCC);

                        if (pFunc(manipInfo->unkPtr, resolvedObject))
                        {
                            void** currentStackTop = (void**)manipInfo->unkPtr->unk4;
                            if (currentStackTop >= (void**)manipInfo->unkPtr->weakPtrPtr)
                            {
                                typedef void (*Func5B2BB0)(void*, void**);
                                Func5B2BB0 gFunc = (Func5B2BB0)getActualOffset(0x5B2BB0);
                                gFunc((void*)&manipInfo->unkPtr->unk3, &resolvedObject);
                            }
                            else
                            {
                                *currentStackTop = resolvedObject;
                                manipInfo->unkPtr->weakPtrPtr++;
                            }
                            typedef int(*GetSizeFunc)(UnkStruct*, void*);
                            GetSizeFunc getSize = (GetSizeFunc)getActualOffset(0x14FEE04);
                            int vecSize = getSize(manipInfo->unkPtr, resolvedObject);

                            LOGI("Vector Size from file: %lld", vecSize);

                            if (vecSize > 0)
                            {
                                vector->resize(vecSize);

                                for (int i = 0; i < vecSize; i++)
                                {
                                    T* element = &vector->at(i);
                                    SexyString str;
                                    reflectionType->Function11(element, manipInfo, str);
                                    SexyString* val = (SexyString*)element;
                                    SexyString* val1 = &str;
                                    LOGI("Element [%lld]: '%s'", i, val->c_str());
                                    LOGI("Element [%lld]: '%s'", i, val1->c_str());
                                }
                            }
                            manipInfo->unkPtr->weakPtrPtr--;
                            return true;
                        }
                        return false;
                }
				else
				{
					int64_t** v19 = *(int64_t***)(manipInfo + 8);
					typedef void (*sub254104)(int64_t**, Sexy::RtWeakPtr<void>*, int, char);
					((sub254104)getActualOffset(OFFSET_REFLECTION_RCUSTOMTYPE_TSTDVECTORMANIPULATOR_FUNC2))(v19, unkWeakPtr, vector->size(), 0xFD);

					if (!vector->empty()) {
						for (size_t i = 0; i < vector->size(); ++i) {
							T* element = &vector->at(i);
							SexyString str;
							reflectionType->Function11(element, manipInfo, str);
						}
					}

					int64_t* v17 = *v19;
					v19[2] = (int64_t*)((char*)v19[2] - 1);

					uint8_t v21 = 254;
					typedef int (*sub10E5340)(int64_t*, uint8_t*, int);
					((sub10E5340)getActualOffset(OFFSET_REFLECTION_RCUSTOMTYPE_TSTDVECTORMANIPULATOR_FUNC3))(v17, &v21, 1);

					return true;

					LOGI("[ Reflection::RCustomType::TStdVectorManipulator::Edit ] [ Panic! Need to edit vector but manipInfo->unkBool is false ]");
				}
			}

			virtual int GetSize(GenericVector* vector)
			{
				return vector->size();
			}
		};
	}
}