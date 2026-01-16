#pragma once
#include "Sexy/RtWeakPtr.h"
#include "Reflection/RType.h"

namespace Reflection::RCustomType
{
	struct UnkStruct
	{
	public:

		int m_unk1;
		int m_unk2;
		int m_unk3;
		int m_unk4;
		Sexy::RtWeakPtr<void>* m_weakPtrPtr;
		uintptr_t** m_stackPtr;
		uintptr_t** m_stackEnd;
	};

	struct VectorManipulatorStruct
	{
	public:
		UnkStruct* m_unkPtr;
		void* m_unkPtr2;
		bool m_unkBool;
	};

	template <typename T>
	class TStdVectorManipulator
	{
	public:
		using GenericVector = std::vector<T>;

		virtual void NullSub() {};

		void DestroyEffect(void* a1)
		{
			operator delete(a1);
		}

		bool Edit(GenericVector* vector, VectorManipulatorStruct* manipInfo, Sexy::RtWeakPtr<void>* unkWeakPtr, RType* reflectionType)
		{

			if (manipInfo->m_unkBool)
			{
				UnkStruct* unkPtr = manipInfo->m_unkPtr;

				typedef uintptr_t (*FuncCheck)(UnkStruct*, Sexy::RtWeakPtr<void>*);
				FuncCheck pFuncCheck = (FuncCheck)getActualOffset(0x5B2AA4);
				uintptr_t checkResult = pFuncCheck(unkPtr, unkWeakPtr);

				if (checkResult)
				{
					typedef bool (*FuncVerify)(UnkStruct*, int);
					FuncVerify pFuncVerify = (FuncVerify)getActualOffset(0x14FEDCC);

					if (pFuncVerify(unkPtr, checkResult))
					{
						if (unkPtr->m_stackPtr >= unkPtr->m_stackEnd)
						{
							typedef void (*FuncPush)(void*, uintptr_t*);
							((FuncPush)getActualOffset(0x5B2BB0))(&unkPtr->m_weakPtrPtr, &checkResult);
						}
						else
						{
							*unkPtr->m_stackPtr = (uintptr_t*)checkResult;
							unkPtr->m_stackPtr++;
						}

						typedef uintptr_t(*FuncGetSize)(UnkStruct*, int);
						uintptr_t vecSize = ((FuncGetSize)getActualOffset(0x14FEE04))(unkPtr, checkResult);

						if (vecSize > 0)
						{
							vector->resize(vecSize);

							for (int i = 0; i < vecSize; i++)
							{
								auto* element = &vector->at(i);
								SexyString str;
								reflectionType->Function11(element, manipInfo, str);
							}
						}

						unkPtr->m_stackPtr--;
						return true;
					}
					return false;
				}
				return false;
			}
			else
			{
				typedef void (*sub5B2440)(void*, Sexy::RtWeakPtr<void>*, int, int);
				((sub5B2440)getActualOffset(0x5B2440))(manipInfo->m_unkPtr2, unkWeakPtr, vector->size(), 253);

				if (!vector->empty()) {
					for (int i = 0; i < vector->size(); ++i) {
						auto* element = &vector->at(i);
						SexyString str;
						reflectionType->Function11(element, manipInfo, str);
					}
				}
				uintptr_t* refCount = (uintptr_t*)((char*)manipInfo->m_unkPtr2 + 16);
				(*refCount)--;

				void* objPtr = *(void**)manipInfo->m_unkPtr2;
				uint8_t v21 = 254;

				typedef uintptr_t(*sub14F8794)(void*, uint8_t*, int);
				((sub14F8794)getActualOffset(0x14F8794))(objPtr, &v21, 1);

				return true;
			}
		}
		int GetSize(GenericVector* vector)
		{
			return vector->size();
		}
	};
}