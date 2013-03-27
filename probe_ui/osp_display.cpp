#include <FUi.h>

#include "daprobe.h"
#include "dahelper.h"
#include "osp_probe.h"

namespace Tizen { namespace Ui { namespace Animations {

class _RootVisualElement
{
};

class _DisplayManager
{
//	bool RenderAll(bool check);
//	bool Render(_RootVisualElement& root, bool check);
	result PostRender(_RootVisualElement& root);
//	result Flush(void);
//	bool UpdateScene(bool force);
};
/*
bool _DisplayManager::RenderAll(bool check)
{
	typedef bool (_DisplayManager::*methodtype)(bool check);
	static methodtype mp;
	bool ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui10Animations15_DisplayManager9RenderAllEb, libosp-uifw.so, mp);
	probeBlockStart();
	PRINTMSG("Before RenderAll");
	probeBlockEnd();

	ret = (this->*mp)(check);

	probeBlockStart();
	PRINTMSG("After RenderAll");
	probeBlockEnd();

	return ret;
}

bool _DisplayManager::Render(_RootVisualElement& root, bool check)
{
	typedef bool (_DisplayManager::*methodtype)(_RootVisualElement& root, bool check);
	static methodtype mp;
	bool ret;
	
	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui10Animations15_DisplayManager6RenderERNS1_18_RootVisualElementEb, libosp-uifw.so, mp);
	probeBlockStart();
	PRINTMSG("Before Render");
	probeBlockEnd();
	ret = (this->*mp)(root, check);
	probeBlockStart();
	PRINTMSG("After Render");
	probeBlockEnd();

	return ret;
}
*/
result _DisplayManager::PostRender(_RootVisualElement& root)
{
	typedef result (_DisplayManager::*methodtype)(_RootVisualElement& root);
	static methodtype mp;
	result ret;
	
	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui10Animations15_DisplayManager10PostRenderERNS1_18_RootVisualElementE, libosp-uifw.so, mp);
	probeBlockStart();
//	PRINTMSG("Before PostRender");
	probeBlockEnd();

	ret = (this->*mp)(root);

	probeBlockStart();
	SCREENSHOT_DONE();
//	PRINTMSG("After PostRender");
	probeBlockEnd();

	return ret;
}
/*
result _DisplayManager::Flush(void)
{
	typedef result (_DisplayManager::*methodtype)(void);
	static methodtype mp;
	result ret;
	
	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui10Animations15_DisplayManager5FlushEv, libosp-uifw.so, mp);
	probeBlockStart();
	PRINTMSG("Before Flush");
	probeBlockEnd();
	ret = (this->*mp)();
	probeBlockStart();
	PRINTMSG("After Flush");
	probeBlockEnd();

	return ret;
}

bool _DisplayManager::UpdateScene(bool force)
{
	typedef bool (_DisplayManager::*methodtype)(bool force);
	static methodtype mp;
	bool ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui10Animations15_DisplayManager11UpdateSceneEb, libosp-uifw.so, mp);
	probeBlockStart();
	PRINTMSG("Before UpdateScene");
	probeBlockEnd();
	ret = (this->*mp)(force);
	probeBlockStart();
	PRINTMSG("After UpdateScene");
	probeBlockEnd();

	return ret;
}
*/

} } }	// end of namespace
