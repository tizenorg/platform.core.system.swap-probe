/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * Anastasia Lyupa <a.lyupa@samsung.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 * - Samsung RnD Institute Russia
 *
 */

#include <assert.h>
#include <FBase.h>
#include <FUi.h>

#include "daprobe.h"
#include "dacollection.h"	// for find_object_hash
#include "dahelper.h"		// for captureScreen
#include "osp_probe.h"

#include "binproto.h"

using namespace Tizen::Base;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

int SceneManagerUsed = 0;

class SceneManagerEventListener : public ISceneManagerEventListener
{
public:
	virtual void OnSceneTransitionCompleted(const SceneId &previousSceneId, const SceneId &currentSceneId);
	virtual void OnSceneTransitionStarted(const SceneId &currentSceneId, const SceneId &nextSceneId);
	static SceneManagerEventListener& GetInstance();
private:
	static SceneManagerEventListener rInstance;
	unsigned long startTime;
};

SceneManagerEventListener SceneManagerEventListener::rInstance;

void SceneManagerEventListener::OnSceneTransitionCompleted(const SceneId &previousSceneId, const SceneId &currentSceneId)
{
	probeBlockStart();
	{
		SceneManager* scenemanager = SceneManager::GetInstance();

		if(scenemanager->GetCurrentSceneId() == currentSceneId)
		{
			if(isOptionEnabled(OPT_UI))
			{
				probeInfo_t	probeInfo;
				Scene* scene;
				String sceneid, formid, panelid;
				Form* pform;
				Panel* ppanel;
				unsigned long transition, user = 0;

				setProbePoint(&probeInfo);

				scene = scenemanager->GetCurrentScene();
				sceneid = scene->GetSceneId();
				formid = scene->GetFormId();
				panelid = scene->GetPanelId();
				pform = scene->GetForm();
				ppanel = scene->GetPanel();

				transition = (probeInfo.currentTime - startTime);

				char scene_name[50];
				WcharToChar(scene_name, sceneid.GetPointer());
				
				PREPARE_LOCAL_BUF();
				PACK_COMMON_BEGIN(MSG_PROBE_SCENE, LC_SCENE, "pp", &previousSceneId, &currentSceneId);
				PACK_COMMON_END(0, 0, 0);
				PACK_SCENE(scene_name, formid, pform, panelid, ppanel, transition, user);
				FLUSH_LOCAL_BUF();
			}
		}
		else
		{
			// never happened
			assert(false);
		}

		SCREENSHOT_SET();
//		SCREENSHOT_DONE();
	}

	SceneManagerUsed = 0;
	probeBlockEnd();
}

void SceneManagerEventListener::OnSceneTransitionStarted(const SceneId &currentSceneId, const SceneId &nextSceneId)
{
	probeBlockStart();
	SceneManagerUsed = 1;
	startTime = getCurrentTime();
	probeBlockEnd();
}

SceneManagerEventListener& SceneManagerEventListener::GetInstance()
{
	return rInstance;
}


namespace Tizen { namespace Ui { namespace Scenes {

SceneManager* SceneManager::GetInstance(void)
{
	typedef SceneManager* (*methodType)(void);
	static methodType scenemanager_getinstancep;
	static int initialized = 0;
	SceneManager* ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui6Scenes12SceneManager11GetInstanceEv, LIBOSP_UIFW, scenemanager_getinstancep);

	ret = scenemanager_getinstancep();

	probeBlockStart();
	if(unlikely(initialized == 0))
	{
		if(likely(ret != NULL))
		{
			ret->AddSceneManagerEventListener(SceneManagerEventListener::GetInstance());
			initialized = 1;
		}
	}
	probeBlockEnd();

	return ret;
}


} } }	// end of namespace
