#pragma once
class PTXInfo;

namespace Sexy {
	class IResStreamDriver
	{
	public:
		virtual ~IResStreamDriver() {};
		virtual void Func2() {};
		virtual void Func3() {};
		virtual void Func4() {};
		virtual void Func5() {};
		virtual void Func6() {};
		virtual void Func7() {};
		virtual void Func8() {};
		virtual void Func9() {};
		virtual void Func10() {};
		virtual void Func11() {};
		virtual void Func12() {};
		virtual void* CreateReadTask() {};
		virtual void* CreateReadTaskWithoutBuffer() {};
		virtual void Func15() {};
		virtual void* CreateDecompressionTask() {};
		virtual unsigned int GetGLTextureTotalSize(PTXInfo* ptxInfo) {}; // 17
		virtual unsigned int GetGLTextureRGBSize(PTXInfo* ptxInfo) {}; // 18
		virtual unsigned int GetGLTextureAlphaSize(PTXInfo* ptxInfo) {}; // 19
		virtual void* CreateDecodeTask() {};
		virtual int GetReadBufferSize() {};
		virtual int GetDecompressBufferSize() {};
		virtual void Func23() {};
		virtual void OnEffectCreated() {};
		virtual void Func25() {};
		virtual void Func26() {};
		virtual void Func27() {};
		virtual void Func28() {};
		virtual void Func29() {};
		virtual void Func30() {};
	};

	class AndroidResStreamDriver : public IResStreamDriver {};
}