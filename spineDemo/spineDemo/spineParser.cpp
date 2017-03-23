#include "spine-sfml.h"
#include "spineParser.h"

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

static AnimationStateData* stateData;
static AnimationState* state;
static SkeletonData* skeletonData;
static Skeleton* skeleton;
static float* worldVertices;

static SkeletonData* readSkeletonJsonData(const char* filename, Atlas* atlas, float scale) {
	SkeletonJson* json = SkeletonJson_create(atlas);
	json->scale = scale;
	SkeletonData* skeletonData = SkeletonJson_readSkeletonDataFile(json, filename);
	if (!skeletonData) {
		printf("%s\n", json->error);
		exit(0);
	}
	SkeletonJson_dispose(json);
	return skeletonData;
}

static SkeletonData* readSkeletonBinaryData(const char* filename, Atlas* atlas, float scale) {
	SkeletonBinary* binary = SkeletonBinary_create(atlas);
	binary->scale = scale;
	SkeletonData *skeletonData = SkeletonBinary_readSkeletonDataFile(binary, filename);
	if (!skeletonData) {
		printf("%s\n", binary->error);
		exit(0);
	}
	SkeletonBinary_dispose(binary);
	return skeletonData;
}

void _AtlasPage_createTexture(AtlasPage* self, const char* path) {
	//Texture* texture = new Texture();
	//if (!texture->loadFromFile(path)) return;

	//if (self->magFilter == SP_ATLAS_LINEAR) texture->setSmooth(true);
	//if (self->uWrap == SP_ATLAS_REPEAT && self->vWrap == SP_ATLAS_REPEAT) texture->setRepeated(true);

	//self->rendererObject = texture;
	//Vector2u size = texture->getSize();
	//self->width = size.x;
	//self->height = size.y;
}

void _AtlasPage_disposeTexture(AtlasPage* self) {
	//delete (Texture*)self->rendererObject;
}

char* _Util_readFile(const char* path, int* length) {
	return _readFile(path, length);
}


spineParser::spineParser()
{

}


spineParser::~spineParser()
{
	FREE(worldVertices);
	AnimationStateData_dispose(state->data);
	AnimationState_dispose(state);
	Skeleton_dispose(skeleton);
	SkeletonData_dispose(skeletonData);
}


void spineParser::initSpine(const char* name)
{
	if (QString("stretchyman").compare(name) == 0) {
		initStretchyMan();
	} else if (QString("tank").compare(name) == 0) {
		initTank();
	} else if (QString("raptor").compare(name) == 0) {
		initRaptor();
	} else if (QString("goblins").compare(name) == 0){
		initgoblins();
	} else if (QString("spineboy").compare(name) == 0) {
		initspineboy();
	}
}

void spineParser::update(float deltaTime)
{
	Skeleton_update(skeleton, deltaTime);
	AnimationState_update(state, deltaTime * 1);
	AnimationState_apply(state, skeleton);
	Skeleton_updateWorldTransform(skeleton);
}

QVector<SpineVertex> spineParser::parseSpineVertex()
{
	int rootBoneX = skeleton->x;
	int rootBoneY = skeleton->y;

	QVector<SpineVertex> vertexs;
	for (int i = 0; i < skeleton->slotsCount; ++i) {
		Slot* slot = skeleton->drawOrder[i];
		Attachment* attachment = slot->attachment;
		if (!attachment) continue;

		//sf::BlendMode blend;
		//switch (slot->data->blendMode) {
		//case BLEND_MODE_ADDITIVE:
		//	blend = BlendAdd;
		//	break;
		//case BLEND_MODE_MULTIPLY:
		//	blend = BlendMultiply;
		//	break;
		//case BLEND_MODE_SCREEN: // Unsupported, fall through.
		//default:
		//	blend = BlendAlpha;
		//}
		//if (states.blendMode != blend) {
		//	target.draw(*vertexArray, states);
		//	vertexArray->clear();
		//	states.blendMode = blend;
		//}

		if (attachment->type == ATTACHMENT_REGION) {
			RegionAttachment* regionAttachment = (RegionAttachment*)attachment;
			RegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

			quint8 r = static_cast<quint8>(skeleton->r * slot->r * 255);
			quint8 g = static_cast<quint8>(skeleton->g * slot->g * 255);
			quint8 b = static_cast<quint8>(skeleton->b * slot->b * 255);
			quint8 a = static_cast<quint8>(skeleton->a * slot->a * 255);

			SpineVertex vertexA;
			vertexA.color = QVector4D(r, g, b, a);
			vertexA.pos = QVector3D(worldVertices[VERTEX_X1], worldVertices[VERTEX_Y1], 0);
			vertexA.texCord = QVector2D(regionAttachment->uvs[VERTEX_X1], regionAttachment->uvs[VERTEX_Y1]);

			SpineVertex vertexB;
			vertexB.color = QVector4D(r, g, b, a);
			vertexB.pos = QVector3D(worldVertices[VERTEX_X2], worldVertices[VERTEX_Y2], 0);
			vertexB.texCord = QVector2D(regionAttachment->uvs[VERTEX_X2], regionAttachment->uvs[VERTEX_Y2]);

			SpineVertex vertexC;
			vertexC.color = QVector4D(r, g, b, a);
			vertexC.pos = QVector3D(worldVertices[VERTEX_X3], worldVertices[VERTEX_Y3], 0);
			vertexC.texCord = QVector2D(regionAttachment->uvs[VERTEX_X3], regionAttachment->uvs[VERTEX_Y3]);

			SpineVertex vertexD;
			vertexD.color = QVector4D(r, g, b, a);
			vertexD.pos = QVector3D(worldVertices[VERTEX_X4], worldVertices[VERTEX_Y4], 0);
			vertexD.texCord = QVector2D(regionAttachment->uvs[VERTEX_X4], regionAttachment->uvs[VERTEX_Y4]);

			vertexs.append(vertexA);
			vertexs.append(vertexB);
			vertexs.append(vertexC);
			vertexs.append(vertexA);
			vertexs.append(vertexC);
			vertexs.append(vertexD);
		}
		else if (attachment->type == ATTACHMENT_MESH) {
			MeshAttachment* mesh = (MeshAttachment*)attachment;
			if (mesh->super.worldVerticesLength > SPINE_MESH_VERTEX_COUNT_MAX) continue;
			MeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

			quint8 r = static_cast<quint8>(skeleton->r * slot->r * 255);
			quint8 g = static_cast<quint8>(skeleton->g * slot->g * 255);
			quint8 b = static_cast<quint8>(skeleton->b * slot->b * 255);
			quint8 a = static_cast<quint8>(skeleton->a * slot->a * 255);


			for (int i = 0; i < mesh->trianglesCount; ++i) {
				int index = mesh->triangles[i] << 1;
				SpineVertex vertex;
				vertex.color = QVector4D(r, g, b, a);
				vertex.pos = QVector3D(worldVertices[index], worldVertices[index + 1], 1);
				vertex.texCord = QVector2D(mesh->uvs[index], mesh->uvs[index + 1]);
				//vertex.texCoords.x = mesh->uvs[index] * size.x;
				//vertex.texCoords.y = mesh->uvs[index + 1] * size.y;

				vertexs.append(vertex);
			}
		}
	}

	return vertexs;
}

void spineParser::initStretchyMan()
{
	const char* jsonName = "./data/stretchyman.json";
	const char* binaryName = "./data/stretchyman.skel";
	const char* atlasName = "./data/stretchyman.atlas";
	float scale = 1.0f;
	Atlas* atlas = Atlas_createFromFile(atlasName, 0);
	skeletonData = readSkeletonJsonData(jsonName, atlas, scale);

	//Bone_setYDown(true);

	float timeScale = 1;
	bool ownsAnimationStateData = true;
	stateData = AnimationStateData_create(skeletonData);
	state = AnimationState_create(stateData);
	worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);

	skeleton = Skeleton_create(skeletonData);
	skeleton->flipX = false;
	skeleton->flipY = false;
	//Skeleton_setSkinByName(skeleton, "goblin");
	//Skeleton_setSlotsToSetupPose(skeleton);

	skeleton->x = 0;
	skeleton->y = 0;
	Skeleton_updateWorldTransform(skeleton);

	AnimationState_setAnimationByName(state, 0, "sneak", true);

	//skeletonData = readSkeletonBinaryData(binaryName, atlas, scale);
	//proxy.renderSpine(skeletonData, atlas);
	//SkeletonData_dispose(skeletonData);
	//Atlas_dispose(atlas);
}

void spineParser::initTank()
{
	const char* jsonName = "./data/tank.json";
	const char* binaryName = "./data/tank.skel";
	const char* atlasName = "./data/tank.atlas";
	float scale = 0.4f;
	Atlas* atlas = Atlas_createFromFile(atlasName, 0);
	skeletonData = readSkeletonJsonData(jsonName, atlas, scale);

	//Bone_setYDown(true);

	float timeScale = 1;
	bool ownsAnimationStateData = true;
	stateData = AnimationStateData_create(skeletonData);
	state = AnimationState_create(stateData);
	worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);

	skeleton = Skeleton_create(skeletonData);
	skeleton->flipX = false;
	skeleton->flipY = false;

	skeleton->x = 0;
	skeleton->y = 0;
	Skeleton_updateWorldTransform(skeleton);

	AnimationState_setAnimationByName(state, 0, "drive", true);
}

void spineParser::initRaptor()
{
	const char* jsonName = "./data/raptor.json";
	const char* binaryName = "./data/raptor.skel";
	const char* atlasName = "./data/raptor.atlas";
	float scale = 1.0f;
	Atlas* atlas = Atlas_createFromFile(atlasName, 0);
	skeletonData = readSkeletonJsonData(jsonName, atlas, scale);

	//Bone_setYDown(true);

	float timeScale = 1;
	bool ownsAnimationStateData = true;
	stateData = AnimationStateData_create(skeletonData);
	state = AnimationState_create(stateData);
	worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);

	skeleton = Skeleton_create(skeletonData);
	skeleton->flipX = false;
	skeleton->flipY = false;

	skeleton->x = 0;
	skeleton->y = 0;
	Skeleton_updateWorldTransform(skeleton);

	AnimationState_setAnimationByName(state, 0, "walk", true);
	AnimationState_addAnimationByName(state, 1, "gungrab", false, 2);
}

void spineParser::initgoblins()
{
	const char* jsonName = "./data/goblins-mesh.json";
	const char* binaryName = "./data/goblins-mesh.skel";
	const char* atlasName = "./data/goblins.atlas";
	float scale = 1.5f;
	Atlas* atlas = Atlas_createFromFile(atlasName, 0);
	skeletonData = readSkeletonJsonData(jsonName, atlas, scale);

	//Bone_setYDown(true);

	float timeScale = 1;
	bool ownsAnimationStateData = true;
	stateData = AnimationStateData_create(skeletonData);
	state = AnimationState_create(stateData);
	worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);

	skeleton = Skeleton_create(skeletonData);
	skeleton->flipX = false;
	skeleton->flipY = false;
	Skeleton_setSkinByName(skeleton, "goblin");
	Skeleton_setSlotsToSetupPose(skeleton);

	skeleton->x = 0;
	skeleton->y = 0;
	Skeleton_updateWorldTransform(skeleton);

	AnimationState_setAnimationByName(state, 0, "walk", true);
}

void spineParser::initspineboy()
{
	const char* jsonName = "./data/goblins-mesh.json";
	const char* binaryName = "./data/goblins-mesh.skel";
	const char* atlasName = "./data/goblins.atlas";
	float scale = 1.5f;
	Atlas* atlas = Atlas_createFromFile(atlasName, 0);
	skeletonData = readSkeletonJsonData(jsonName, atlas, scale);

	//Bone_setYDown(true);

	float timeScale = 1;
	bool ownsAnimationStateData = true;
	stateData = AnimationStateData_create(skeletonData);
	AnimationStateData_setMixByName(stateData, "walk", "jump", 0.2f);
	AnimationStateData_setMixByName(stateData, "jump", "run", 0.2f);
	state = AnimationState_create(stateData);
	worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);

	skeleton = Skeleton_create(skeletonData);
	skeleton->flipX = false;
	skeleton->flipY = false;
	Skeleton_setToSetupPose(skeleton);

	skeleton->x = 0;
	skeleton->y = 0;
	Skeleton_updateWorldTransform(skeleton);

	AnimationState_setAnimationByName(state, 0, "test", true);
	AnimationState_addAnimationByName(state, 0, "walk", true, 0);
	AnimationState_addAnimationByName(state, 0, "jump", false, 3);
	AnimationState_addAnimationByName(state, 0, "run", true, 0);
}
