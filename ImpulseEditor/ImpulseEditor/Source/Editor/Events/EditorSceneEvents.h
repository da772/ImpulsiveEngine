#pragma once

#include "EditorEvents.h"

namespace Editor {

	enum class GameObjectModifications : uint32_t {
		NONE = 0x00, ADD_CHILD, SET_PARENT, REMOVE_CHILD, ADD_COMPONENT, REMOVE_COMPONENT, SET_TAG, EDIT_MEMBER 
	};


	class EditorSceneEvent : public EditorEvent {
	public:
		EditorSceneEvent() {};
		EDITOR_EVENT_CLASS_CATEGORY(EventCategoryScene);

	};

	class EditorSceneGameObjectEvent : public EditorSceneEvent {
	public:
		EditorSceneGameObjectEvent(const GEngine::ObjectHash& hash) : hash(hash) {}
		EDITOR_EVENT_CLASS_CATEGORY(EventCategoryScene | EventCategoryGameObject);
		const GEngine::ObjectHash& GetObjectHash() { return hash; }

	protected:
		GEngine::ObjectHash hash;

	};

	class EditorSceneModifyGameObject {
	public:
		EditorSceneModifyGameObject(const GameObjectModifications& type) : type(type) {}
		

	protected:
		GameObjectModifications type;

	};

	class EditorSceneEntityEvent : public EditorSceneGameObjectEvent {
	public:
		EditorSceneEntityEvent(const GEngine::ObjectHash& hash) : EditorSceneGameObjectEvent(hash) {}
		EDITOR_EVENT_CLASS_CATEGORY(EventCategoryScene | EventCategoryEntity | EventCategoryGameObject);
		const GEngine::ObjectHash& GetObjectHash() { return hash; }
	
	};

	class EditorSceneComponentEvent : public EditorSceneGameObjectEvent {
	public:
		EditorSceneComponentEvent(const GEngine::ObjectHash& hash) : EditorSceneGameObjectEvent(hash) {}
		EDITOR_EVENT_CLASS_CATEGORY(EventCategoryScene | EventCategoryComponent | EventCategoryGameObject);
		const GEngine::ObjectHash& GetObjectHash() { return hash; }

	};

	class EditorSceneAddEntity : public EditorSceneEntityEvent {
	public:
		EditorSceneAddEntity(const GEngine::ObjectHash& hash) : EditorSceneEntityEvent(hash) {};
		EDITOR_EVENT_CLASS_TYPE(SceneCreateEntity);

	};

	class EditorSceneDestroyEntity : public EditorSceneEntityEvent {
	public:
		EditorSceneDestroyEntity(const GEngine::ObjectHash& hash) : EditorSceneEntityEvent(hash) {};
		EDITOR_EVENT_CLASS_TYPE(SceneDestroyEntity);

	};

	class EditorSceneModifyEntity : public EditorSceneEntityEvent {
	public:
		EditorSceneModifyEntity(const GEngine::ObjectHash& hash, const GameObjectModifications& mod) : EditorSceneEntityEvent(hash), type(mod) {};
		inline const GameObjectModifications& GetModificationType() const { return type; }
		EDITOR_EVENT_CLASS_TYPE(SceneModifyEntity);
	protected:
		GameObjectModifications type;

	};

	class EditorSceneAddComponent : public EditorSceneComponentEvent {
	public:
		EditorSceneAddComponent(const GEngine::ObjectHash& hash) : EditorSceneComponentEvent(hash) {};
		EDITOR_EVENT_CLASS_TYPE(SceneAddComponent);

	};

	class EditorSceneDestroyComponent : public EditorSceneComponentEvent {
	public:
		EditorSceneDestroyComponent(const GEngine::ObjectHash& hash) : EditorSceneComponentEvent(hash) {};
		EDITOR_EVENT_CLASS_TYPE(SceneDestroyComponent);

	};

	class EditorSceneModifyComponent : public EditorSceneComponentEvent {
	public:
		EditorSceneModifyComponent(const GEngine::ObjectHash& hash, const GameObjectModifications& mod) : EditorSceneComponentEvent(hash), type(mod) {};
		inline const GameObjectModifications& GetModificationType() const { return type; }
		EDITOR_EVENT_CLASS_TYPE(SceneModifyEntity);
	protected:
		GameObjectModifications type;


	};

	

}
