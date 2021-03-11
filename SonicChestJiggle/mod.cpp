#include "pch.h"
#include "SA2ModLoader.h"
#include <cmath>

struct JiggleData
{
	JiggleInfo* TorsoJiggle;
	JiggleInfo* TorsoJiggle2;
	JiggleInfo* SuperTorsoJiggle;
	JiggleInfo* SuperTorsoJiggle2;
	NJS_VECTOR TorsoJigglePos;
	NJS_VECTOR TorsoNodePos;
};
JiggleData* jiggledata[2] = {};

FunctionPointer(JiggleInfo*, InitJiggle, (NJS_OBJECT*), 0x447580);

// void __usercall(JiggleInfo *a1@<eax>)
static const void* const ApplyJigglePtr = (void*)0x4479E0;
static inline void ApplyJiggle(JiggleInfo* a1)
{
	__asm
	{
		mov eax, [a1]
		call ApplyJigglePtr
	}
}

// void __usercall(JiggleInfo *a1@<esi>)
static const void* const FreeJigglePtr = (void*)0x447910;
static inline void FreeJiggle(JiggleInfo* a1)
{
	__asm
	{
		mov esi, [a1]
		call FreeJigglePtr
	}
}

FunctionPointer(void, sub_71EAA0, (NJS_OBJECT*), 0x71EAA0);
DataPointer(SonicCharObj2*, dword_1A51A9C, 0x1A51A9C);
DataArray(float, flt_1A51A00, 0x1A51A00, 12);
void SonicJiggleHook(NJS_OBJECT* obj)
{
	sub_71EAA0(obj);
	if (obj == CharacterModels[1].Model)
	{
		NJS_VECTOR a3{};
		if (jiggledata[dword_1A51A9C->base.PlayerNum])
		{
			njCalcVector(_nj_current_matrix_ptr_, &a3, &a3, 0);
			njCalcVector(flt_1A51A00, &jiggledata[dword_1A51A9C->base.PlayerNum]->TorsoNodePos, &a3, 0);
		}
	}
}

FunctionPointer(void, sub_49C130, (NJS_OBJECT*), 0x49C130);
DataPointer(SonicCharObj2*, dword_19EE150, 0x19EE150);
DataArray(float, flt_19EE0C0, 0x19EE0C0, 12);
void SuperJiggleHook(NJS_OBJECT* obj)
{
	sub_49C130(obj);
	if (obj == CharacterModels[1].Model || obj == CharacterModels[329].Model)
	{
		NJS_VECTOR a3{};
		if (jiggledata[dword_19EE150->base.PlayerNum])
		{
			njCalcVector(_nj_current_matrix_ptr_, &a3, &a3, 0);
			njCalcVector(flt_19EE0C0, &jiggledata[dword_19EE150->base.PlayerNum]->TorsoNodePos, &a3, 0);
		}
	}
}

void SonicDisplayHook(ObjectMaster* obj)
{
	NJS_CNK_MODEL* tm1 = nullptr;
	NJS_CNK_MODEL* tm2 = nullptr;
	if (obj->Data2.Character->CharID2 == Characters_Sonic && jiggledata[obj->Data2.Character->PlayerNum])
	{
		tm1 = CharacterModels[2].Model->child->chunkmodel;
		CharacterModels[2].Model->child->chunkmodel = jiggledata[obj->Data2.Character->PlayerNum]->TorsoJiggle->SourceModelCopy->chunkmodel;
		tm2 = CharacterModels[2].Model->child->child->chunkmodel;
		CharacterModels[2].Model->child->child->chunkmodel = jiggledata[obj->Data2.Character->PlayerNum]->TorsoJiggle2->SourceModelCopy->chunkmodel;
	}
	Sonic_Display(obj);
	if (obj->Data2.Character->CharID2 == Characters_Sonic && jiggledata[obj->Data2.Character->PlayerNum])
	{
		CharacterModels[2].Model->child->chunkmodel = tm1;
		CharacterModels[2].Model->child->child->chunkmodel = tm2;
	}
}

void SuperDisplayHook(ObjectMaster* obj)
{
	NJS_CNK_MODEL* tm1 = nullptr;
	NJS_CNK_MODEL* tm2 = nullptr;
	NJS_CNK_MODEL* sm1 = nullptr;
	NJS_CNK_MODEL* sm2 = nullptr;
	if (obj->Data2.Character->CharID2 == Characters_SuperSonic && jiggledata[obj->Data2.Character->PlayerNum])
	{
		tm1 = CharacterModels[2].Model->child->chunkmodel;
		CharacterModels[2].Model->child->chunkmodel = jiggledata[obj->Data2.Character->PlayerNum]->TorsoJiggle->SourceModelCopy->chunkmodel;
		tm2 = CharacterModels[2].Model->child->child->chunkmodel;
		CharacterModels[2].Model->child->child->chunkmodel = jiggledata[obj->Data2.Character->PlayerNum]->TorsoJiggle2->SourceModelCopy->chunkmodel;
		sm1 = CharacterModels[330].Model->child->chunkmodel;
		CharacterModels[330].Model->child->chunkmodel = jiggledata[obj->Data2.Character->PlayerNum]->SuperTorsoJiggle->SourceModelCopy->chunkmodel;
		sm2 = CharacterModels[330].Model->child->child->chunkmodel;
		CharacterModels[330].Model->child->child->chunkmodel = jiggledata[obj->Data2.Character->PlayerNum]->SuperTorsoJiggle2->SourceModelCopy->chunkmodel;
	}
	Super_Display(obj);
	if (obj->Data2.Character->CharID2 == Characters_SuperSonic && jiggledata[obj->Data2.Character->PlayerNum])
	{
		CharacterModels[2].Model->child->chunkmodel = tm1;
		CharacterModels[2].Model->child->child->chunkmodel = tm2;
		CharacterModels[330].Model->child->chunkmodel = sm1;
		CharacterModels[330].Model->child->child->chunkmodel = sm2;
	}
}

extern "C"
{
	__declspec(dllexport) void OnFrame()
	{
		for (int i = 0; i < 2; i++)
			if (MainCharObj2[i] && (MainCharObj2[i]->CharID2 == Characters_Sonic || MainCharObj2[i]->CharID2 == Characters_SuperSonic))
			{
				if (!jiggledata[i])
				{
					jiggledata[i] = new JiggleData();
					jiggledata[i]->TorsoJiggle2 = InitJiggle(CharacterModels[2].Model->child->child);
					jiggledata[i]->TorsoJiggle2->type = 14;
					jiggledata[i]->TorsoJiggle2->speed = 0.40000001f;
					jiggledata[i]->TorsoJiggle2->field_8 = 12288;
					jiggledata[i]->TorsoJiggle2->field_10 = 1024;
					jiggledata[i]->TorsoJiggle2->Model = CharacterModels[2].Model->child->child;
					jiggledata[i]->TorsoJiggle = InitJiggle(CharacterModels[2].Model->child);
					jiggledata[i]->TorsoJiggle->type = 13;
					jiggledata[i]->TorsoJiggle->speed = 0.40000001f;
					jiggledata[i]->TorsoJiggle->field_8 = 12288;
					jiggledata[i]->TorsoJiggle->field_10 = 1024;
					jiggledata[i]->TorsoJiggle->Model = CharacterModels[2].Model->child;
					jiggledata[i]->TorsoJigglePos = MainCharObj1[i]->Position;
					if (MainCharObj2[i]->CharID2 == Characters_SuperSonic)
					{
						jiggledata[i]->SuperTorsoJiggle2 = InitJiggle(CharacterModels[330].Model->child->child);
						jiggledata[i]->SuperTorsoJiggle2->type = 14;
						jiggledata[i]->SuperTorsoJiggle2->speed = 0.40000001f;
						jiggledata[i]->SuperTorsoJiggle2->field_8 = 12288;
						jiggledata[i]->SuperTorsoJiggle2->field_10 = 1024;
						jiggledata[i]->SuperTorsoJiggle2->Model = CharacterModels[330].Model->child->child;
						jiggledata[i]->SuperTorsoJiggle = InitJiggle(CharacterModels[330].Model->child);
						jiggledata[i]->SuperTorsoJiggle->type = 13;
						jiggledata[i]->SuperTorsoJiggle->speed = 0.40000001f;
						jiggledata[i]->SuperTorsoJiggle->field_8 = 12288;
						jiggledata[i]->SuperTorsoJiggle->field_10 = 1024;
						jiggledata[i]->SuperTorsoJiggle->Model = CharacterModels[330].Model->child;
					}
				}
				float v46 = jiggledata[i]->TorsoNodePos.y;
				float v47 = jiggledata[i]->TorsoNodePos.z;
				NJS_VECTOR v115 = { };
				v115.x = jiggledata[i]->TorsoNodePos.x;
				float v48 = v115.x - jiggledata[i]->TorsoJigglePos.x;
				v115.y = v46;
				v115.z = v47;
				v115.x = v48;
				v115.y = v46 - jiggledata[i]->TorsoJigglePos.y;
				v115.z = v47 - jiggledata[i]->TorsoJigglePos.z;
				float v113 = njScalor(&v115);
				float v49 = jiggledata[i]->TorsoNodePos.y;
				float v45 = v113;
				float v50 = jiggledata[i]->TorsoNodePos.z;
				jiggledata[i]->TorsoJigglePos.x = jiggledata[i]->TorsoNodePos.x;
				JiggleInfo* v44 = jiggledata[i]->TorsoJiggle2;
				jiggledata[i]->TorsoJigglePos.y = v49;
				jiggledata[i]->TorsoJigglePos.z = v50;
				float v51 = fabs(v45);
				float v52 = v51;
				float v53 = fabs(v44->speed);
				if (v53 < v52)
				{
					v44->speed = v113;
				}
				ApplyJiggle(jiggledata[i]->TorsoJiggle2);
				v45 = v113;
				JiggleInfo* v54 = jiggledata[i]->TorsoJiggle;
				float v55 = fabs(v45);
				float v56 = v55;
				float v57 = fabs(v54->speed);
				if (v57 < v56)
				{
					v54->speed = v45;
				}
				ApplyJiggle(jiggledata[i]->TorsoJiggle);
				if (MainCharObj2[i]->CharID2 == Characters_SuperSonic)
				{
					v44 = jiggledata[i]->SuperTorsoJiggle2;
					v53 = fabs(v44->speed);
					if (v53 < v52)
					{
						v44->speed = v113;
					}
					ApplyJiggle(jiggledata[i]->SuperTorsoJiggle2);
					v54 = jiggledata[i]->SuperTorsoJiggle;
					v57 = fabs(v54->speed);
					if (v57 < v56)
					{
						v54->speed = v45;
					}
					ApplyJiggle(jiggledata[i]->SuperTorsoJiggle);
				}
			}
			else if (jiggledata[i])
			{
				if (jiggledata[i]->TorsoJiggle)
				{
					FreeJiggle(jiggledata[i]->TorsoJiggle);
					jiggledata[i]->TorsoJiggle = nullptr;
				}
				if (jiggledata[i]->TorsoJiggle2)
				{
					FreeJiggle(jiggledata[i]->TorsoJiggle2);
					jiggledata[i]->TorsoJiggle2 = nullptr;
				}
				delete jiggledata[i];
				jiggledata[i] = nullptr;
			}
	}

	__declspec(dllexport) void Init(const char* path, const HelperFunctions& helperFunctions)
	{
		WriteData((void (**)(NJS_OBJECT*))0x7201CF, SonicJiggleHook);
		WriteData((void (**)(NJS_OBJECT*))0x49CB07, SuperJiggleHook);

		WriteData((void (**)(ObjectMaster*))0x71706F, SonicDisplayHook);
		WriteData((void (**)(ObjectMaster*))0x49AB4F, SuperDisplayHook);
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}