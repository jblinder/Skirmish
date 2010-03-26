/*
 *  Creature.h
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#ifndef _CREATURE
#define _CREATURE

#include "ofMain.h"
#include "ofxVectorMath.h"

#include <iostream>


class Creature {
	
public:
	Creature();
	
	void draw();
	void resetForce();
	void addForce(float x, float y);
	
	void addRepulsionForce(ofxVec2f posOfForce, float radius, float scale);
	void addAttractionForce(float x, float y, float radius, float scale);
	
	void addDampingForce();
	
	void setInitialCondition(float px, float py, float vx, float vy);
	void update();
	void die();
	
	void bounceOffWalls();
	bool checkHit(float f_gunX, float f_gunY);
	
	// SETTERS
	void setAllProperties(string s_creature_type, 
						  float f_damping, 
						  bool b_repel, 
						  float f_radius, 
						  float f_strength, 
						  float f_too_close, 
						  float f_in_sight,
						  float f_scale,
						  string s_sprite,
						  string s_sounds,
						  float f_damage_rate,
						  float f_hit_points);
	
	void setAllProperties(Creature creature_archetype);
	
	// TODO: make setters for individual properties
	
	// GETTERS
	ofxVec2f getPosition();
	float getXPosition();
	float getYPosition();
	float getRadius();
	float getDamage();
	float getStrength();
	float getDamping();
	float getTooClose();
	float getInSight();
	float getScale();
	float getHitPoints();
	bool getRepel();
	string getType();
	string getSprite();
	string getSounds();
	// TODO: get sprite should return png
	// TODO: get sound should return audio
	
protected:
	
	ofxVec2f ofv_pos;
	ofxVec2f ofv_vel;
	ofxVec2f ofv_frc;   // frc is also know as acceleration (newton says "f=ma")
	string ms_creature_type;
	string ms_sprite;
	string ms_sounds;
	bool mb_repel;
	float mf_damage;
	float mf_damping;
	float mf_radius;
	float mf_strength;
	float mf_too_close;
	float mf_in_sight;
	float mf_scale;
	float mf_hit_points;
};

#endif