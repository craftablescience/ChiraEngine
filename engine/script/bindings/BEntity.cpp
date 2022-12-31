/*
	AngelScript Entity Bindings
*/

#include "angelscript.h"
#include <script/AngelScriptVM.h>
#include <script/ASBindings.h>
#include <entity/Entity.h>

using namespace chira;

void ASRef_EntityConstruct(void *memory, std::string name) {
	new(memory) Entity(name);
}

void ASRef_EntityDestruct(void *memory) {
	((Entity*)memory)->~Entity();
}

void chira::ASB_Entity() {
  	asIScriptEngine* engine = AngelScriptVM::getScriptEngine();
  	engine->RegisterObjectType("Entity", sizeof(Entity), asOBJ_VALUE);
    engine->RegisterObjectBehaviour("Entity", asBEHAVE_CONSTRUCT, "void f(string)", asFUNCTION(ASRef_EntityConstruct), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("Entity", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ASRef_EntityDestruct), asCALL_CDECL_OBJLAST);
}