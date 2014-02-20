#include "ModelFileGenerator.h"

void writeBonesTree(FILE* fp, const std::vector<SModelBoneWrapper>& bones)
{
	for (u32 i = 0; i < bones.size(); i++)
	{
		fwrite(&bones[i].Bone, sizeof(SModelBone), 1, fp);
	}

	//fwrite(&bones[0], sizeof(SModelBone), bones.size(), fp);
}

void writeAnimations(FILE* fp, const std::vector<SBoneAnimationClip>& clips)
{
	for (u32 i = 0; i < clips.size(); i++)
	{
		const SBoneAnimationClip& clip = clips[i];
		/* 对每个AnimationClip, 先写入Name */
		fwrite(clip.Name, ANIMATION_CLIP_NAME_MAX_LENGTH, 1, fp);

		/* 接下来16位，写入该Clip含有的骨骼数量 */
		u16 boneCount = clip.BoneAnimations.size();

		fwrite(&boneCount, sizeof(u16), 1, fp);

		for (u32 j = 0; j < clip.BoneAnimations.size(); j++)
		{
			const SBoneAnimation& boneAnimation = clip.BoneAnimations[j];

			/* 对每个BoneAnimation, 首先写入16位的骨骼ID，该ID和骨骼树中的ID对应 */
			u16 boneId = boneAnimation.BoneID;
			fwrite(&boneId, sizeof(u16), 1, fp);

			/* 分别写入平移、缩放、旋转帧的数量 */
			u16 translate_frame_count = boneAnimation.TranslationFrames.size();
			u16 scale_frame_count = boneAnimation.ScaleFrames.size();
			u16 rotate_frame_count = boneAnimation.RotationFrames.size();

			fwrite(&translate_frame_count, sizeof(u16), 1, fp);
			fwrite(&scale_frame_count, sizeof(u16), 1, fp);
			fwrite(&rotate_frame_count, sizeof(u16), 1, fp);

			/* 将平移、缩放、旋转的具体frames写入 */
			fwrite(&boneAnimation.TranslationFrames[0], sizeof(STranslationAnimateFrame), translate_frame_count, fp);
			fwrite(&boneAnimation.ScaleFrames[0], sizeof(SScaleAnimateFrame), scale_frame_count, fp);
			fwrite(&boneAnimation.RotationFrames[0], sizeof(SRotationAnimateFrame), rotate_frame_count, fp);
		}
	}
}

