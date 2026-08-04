#pragma once

#define OFFSET_RTWEAKPTR_GETREGISTRY 0x14EAD90
#define OFFSET_RTWEAKPTR_COPYWEAKPTR 0x14F39CC
#define OFFSET_RTWEAKPTR_GETTABLEOFTYPE 0x14F31A4
#define OFFSET_RTWEAKPTR_EQUALS 0x5AB660
#define OFFSET_RTWEAKPTR_ISVALIDPTR 0x5D8ABC
#define OFFSET_RTWEAKPTR_RESOLVE 0x14EDE00
#define OFFSET_RTWEAKPTR_FROMOTHER 0x14D920C
#define OFFSET_RTWEAKPTR_SETWEAKPTR 0x14D8B08
#define OFFSET_RTWEAKPTR_MOVE 0x14D91C8
#define NULL_RTID 0LL
namespace Sexy
{
    template<typename T>

    class RtWeakPtr
    {
    public:

        int mRtDataPtr;
        int mRtId;

        RtWeakPtr()
        {
            mRtDataPtr = NULL_RTID;
            mRtId = NULL_RTID;
        }

        ~RtWeakPtr()
        {
            typedef void(*setWeakPtr)(RtWeakPtr*, int64_t);
            setWeakPtr setWeakPtrFunc = (setWeakPtr)getActualOffset(OFFSET_RTWEAKPTR_SETWEAKPTR);
            setWeakPtrFunc(this, 0LL);
        }

        RtWeakPtr(RtWeakPtr* other) {
            mRtDataPtr = NULL;
            mRtId = NULL;

            this->FromOther(other);
        }

        RtWeakPtr(RtWeakPtr&& other) {
            mRtDataPtr = NULL;
            mRtId = NULL;

            typedef void(*weakPtrMove)(RtWeakPtr*, RtWeakPtr*);
            weakPtrMove weakPtrMoveFunc = (weakPtrMove)getActualOffset(OFFSET_RTWEAKPTR_MOVE);
            weakPtrMoveFunc(this, &other);
        }

        RtWeakPtr(const RtWeakPtr& other) {
            mRtDataPtr = NULL;
            mRtId = NULL;

            typedef void(*weakPtrFromOther)(RtWeakPtr*, const RtWeakPtr&);
            weakPtrFromOther weakPtrFromOtherFunc = (weakPtrFromOther)getActualOffset(OFFSET_RTWEAKPTR_FROMOTHER);
            weakPtrFromOtherFunc(this, other);
        }

        bool operator==(const RtWeakPtr<T>& other) const
        {
            typedef bool(*equalsOther)(RtWeakPtr<T>*, RtWeakPtr<T>*);
            equalsOther equalsOtherFunc = (equalsOther)getActualOffset(OFFSET_RTWEAKPTR_EQUALS);
            return equalsOtherFunc(this, &other);
        }

        bool operator==(RtWeakPtr<T>& other)
        {
            typedef bool(*equalsOther)(RtWeakPtr<T>*, RtWeakPtr<T>*);
            equalsOther equalsOtherFunc = (equalsOther)getActualOffset(OFFSET_RTWEAKPTR_EQUALS);
            return equalsOtherFunc(this, &other);
        }

        RtWeakPtr<T>& operator=(const RtWeakPtr<T>& other)
        {
            typedef void(*weakPtrFromOther)(RtWeakPtr*, const RtWeakPtr&);
            weakPtrFromOther weakPtrFromOtherFunc = (weakPtrFromOther)getActualOffset(OFFSET_RTWEAKPTR_FROMOTHER);
            weakPtrFromOtherFunc(this, other);

            // This is how I've been using the assignment operator so far
            mRtDataPtr = other.mRtDataPtr;
            mRtId = other.mRtId;

            return *this;
        }

        RtWeakPtr<T>& operator=(RtWeakPtr<T>& other)
        {
            typedef void(*weakPtrFromOther)(RtWeakPtr*, RtWeakPtr&);
            weakPtrFromOther weakPtrFromOtherFunc = (weakPtrFromOther)getActualOffset(OFFSET_RTWEAKPTR_FROMOTHER);
            weakPtrFromOtherFunc(this, other);
        }

        RtWeakPtr<T>& operator=(RtWeakPtr<T>* other)
        {
            typedef void(*weakPtrFromOther)(RtWeakPtr*, RtWeakPtr&);
            weakPtrFromOther weakPtrFromOtherFunc = (weakPtrFromOther)getActualOffset(OFFSET_RTWEAKPTR_FROMOTHER);
            weakPtrFromOtherFunc(this, other);
        }

        RtWeakPtr<T>& operator=(const RtWeakPtr<T>* other)
        {
            typedef void(*weakPtrFromOther)(RtWeakPtr*, RtWeakPtr&);
            weakPtrFromOther weakPtrFromOtherFunc = (weakPtrFromOther)getActualOffset(OFFSET_RTWEAKPTR_FROMOTHER);
            weakPtrFromOtherFunc(this, other);
        }

        bool Equals(RtWeakPtr<T>* other) {
            typedef bool(*equalsOther)(RtWeakPtr<T>*, RtWeakPtr<T>*);
            equalsOther equalsOtherFunc = (equalsOther)getActualOffset(OFFSET_RTWEAKPTR_EQUALS);
            return equalsOtherFunc(this, other);
        }

        bool IsInvalid()
        {
            return mRtDataPtr == NULL;
        }

        bool IsValid()
        {
            typedef bool(*func)(RtWeakPtr<T>*);
            func pFunc = (func)getActualOffset(OFFSET_RTWEAKPTR_ISVALIDPTR);
            return pFunc(this);
        }

        int GetTypeId()
        {
            return (this->mRtId >> 0x11) & 0x3FFF;
        }

        int GetSomething()
        {
            return this->mRtId >> 0x1F;
        }

        void FromOther(RtWeakPtr* other)
        {
            typedef void(*func)(RtWeakPtr*, RtWeakPtr*);
            func pSetValue = (func)getActualOffset(OFFSET_RTWEAKPTR_FROMOTHER);
            pSetValue(this, other);
        }

        // Get the object this points to, if valid.
        T* Get()
        {
            typedef T* (*RtWeakPtrGet)(RtWeakPtr*);
            RtWeakPtrGet funcRtWeakPtrGet = (RtWeakPtrGet)getActualOffset(0x5E0AF4);
            return funcRtWeakPtrGet(this);
        }
    };
}
