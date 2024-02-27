#include "ECS.h"
void Entity::addGroup(Group myGroup) 
{
	groupBitSet[myGroup] = true;
	manager.AddToGroup(this, myGroup);
}
