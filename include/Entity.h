/*!
 * \file include/Entity.h
 * \brief World Entities
 *
 * \author xythobuz
 */

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "SkeletalModel.h"

class Entity {
  public:
    typedef enum {
        MoveTypeWalkNoSwim = -1,
        MoveTypeWalk       = 0,
        MoveTypeNoClipping = 1,
        MoveTypeFly        = 2,
        MoveTypeSwim       = 3
    } MoveType;

    Entity(float p[3], float a[3], int id, long r, unsigned int model);

    void display();
    void move(char movement);
    void print();

    SkeletalModel& getModel();
    void setSkeletalModel(unsigned int model);
    MoveType getMoveType();
    void setMoveType(MoveType m);
    int getObjectId();
    void setAngles(float a[3]);
    float getPos(unsigned int i);
    float getAngle(unsigned int i);
    long getRoom();

    // Animation State
    unsigned long getAnimation();
    void setAnimation(unsigned long index);
    unsigned long getBoneFrame();
    void setBoneFrame(unsigned long index);
    unsigned long getIdleAnimation();
    void setIdleAnimation(unsigned long index);

  private:
    float pos[3];
    float angles[3];
    long room;

    unsigned int skeletalModel;
    MoveType moveType;

    int state;
    int objectId;

    // Animation State
    unsigned long boneFrame;
    unsigned long animationFrame;
    unsigned long idleAnimation;
};

#endif

