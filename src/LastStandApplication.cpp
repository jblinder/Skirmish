/*
 *  LastStandApplication.cpp
 *  openFrameworks
 *
 *  Created by Axel Esquite on 10/10/09.
 *  Copyright 2009 Axel Esquite. All rights reserved.
 *
 */

#include "LastStandApplication.h"
using namespace std;

//--------------------------------------------------------------
void LastStandApplication::setup(){
	
	/********************************************
	 ** LOAD SETTINGS FROM XML
	 ********************************************/
	
	// check if load was successful
	if( XML.loadFile("data.xml") ){
		printf("data.xml loaded! \n");
		
		// It's all gravy - XML loaded now disperse data
		// first load Game Settings, use GAME_SETTINGS as ROOT
		XML.pushTag("GAME_SETTINGS", 0);
			
			// core game
			player_max				= XML.getValue("PLAYER_MAX", 0);	
			frame_speed				= XML.getValue("FRAME_SPEED", 50);
			
			// for game score, but need more params
			gameScore.setFormat(XML.getValue("SCORE_FORMAT", "TIME"), 0, "DIN-Regular.ttf", 30);
			
			// player settings
			player.setPlayerHitPoints(XML.getValue("PLAYER_SETTINGS:HIT_POINTS", 0.0f));
			player.setDamageRadius(XML.getValue("PLAYER_SETTINGS:DAMAGE_RADIUS", 0.0f));
			player.setGunPower(XML.getValue("PLAYER_SETTINGS:GUN_POWER", 0.0f));
		
			// creature settings
			int numCreatureTags	= XML.getNumTags("CREATURE");
			// if there is at least one creature save the settings
			if(numCreatureTags > 0)
			{
				// loop through all of the creatures
				for(int i = 0; i < numCreatureTags; i++)
				{
					XML.pushTag("CREATURE", i);
					
					
					// sets up strength and area that creatures will flock to
					// TODO: this will have to change. creatures will take in player
					// as target and not need to have a radius, just track player
					//b_repel		= true;
					//f_radius	= 1000.0f;
					//f_strength	= 0.2f;
					
					// creature flock distance info :: are these global
					//f_too_close_dist	= 2.0f;
					//f_in_sight_distance = 6.0f;
					
					
					creatureFactory.setNewCreatureArchetype(XML.getValue("NAME", "DEFAULT"), 
															XML.getValue("DAMPING", 0.0f), 
															XML.getValue("REPEL", true), 
															XML.getValue("RADIUS", 0.0f), 
															XML.getValue("STRENGTH", 0.0f),
															XML.getValue("TOO_CLOSE_DISTANCE", 2.0f),
															XML.getValue("IN_SIGHT_DISTANCE", 6.0F),
															XML.getValue("SCALE", 0.2f),
															XML.getValue("SPRITE", "DEFAULT"),
															XML.getValue("SOUNDS", "DEFAULT"),
															XML.getValue("DAMAGE_RATE", 10.0f),
															XML.getValue("HIT_POINTS", 10.0f));
					
					XML.popTag();
				}
				
			}
		
		// End of GAME_SETTINGS as ROOT
		XML.popTag();
		
	}else{
		printf("unable to load data.xml check data/ folder \n");
	}
	
	
	
	
	// TODO: possibly create a GUI class that handles menus
	// rendering bg and sprites?
	ofBackground(255,255,255);
	
	// TODO: get rid of this when sprites are in
	ofEnableSmoothing();
	ofSetCircleResolution(30);
	
	// TODO: define this in XML
	ofSetFrameRate(frame_speed);
	
	screen_width = ofGetScreenWidth();
	screen_height = ofGetScreenHeight();
	
	player.setPlayerPosition(ofGetWidth()/2, ofGetHeight()/2);
	camera.setupCV();
	
	b_start_game			= false;
	b_adjusting_point		= false;
	i_which_point_adjusting	= 0;

}

//--------------------------------------------------------------
void LastStandApplication::update(){
	
	if(b_start_game == true)
	{
		// update timer
		i_timer = ofGetElapsedTimeMillis() - i_start_time;
		i_timer /= 1000;
		
		// update score type TIMER in seconds
		gameScore.update(i_timer);
		
		if(i_timer %5 == 0)
		{
			
			// sets max creatures allowed on the field
			creature_max = i_timer / 5;
		}
		
		// this will set max creatures allowed on the field
		// then calls spawnCreatures() internally
		// TODO: the creature creation algorithm should be done 
		// solely in the factory; create a smarter algorithm
		creatureFactory.updateCreatureMax(creature_max);
		
		// updates creature flock 
		creatureFactory.updateCreaturesFlock(player);
		b_start_game = player.isAlive();
	
		// looks for ir blobs
		camera.processFrame();
		
		// Assumes there is always a blob
		// sets position of player to blob's (if blob is big enough to be registered as a player)
		player.setPlayerPosition((float) (camera.getPlayerBlobX() * 3.2), (float) (camera.getPlayerBlobY() * 3.2));
		creatureFactory.checkBulletPosition(camera.getBulletX(), camera.getBulletY());
	}
}

//--------------------------------------------------------------
void LastStandApplication::draw(){
	
	// draw player
	player.draw();
	
	// draws all camera views
	//camera.drawFeed();
	
	// your score
	gameScore.outputAsString(730, 40);
	
	// if game is started
	if(b_start_game == true)
	{
		// draw creature flock
		creatureFactory.drawCreatures();
	}
	
}

//--------------------------------------------------------------
void LastStandApplication::keyPressed(int key){
	
	// check if we're setting warp points
	// check if game is starting
	switch (key){
		case '1':
			b_adjusting_point = true;
			i_which_point_adjusting = 0;
			break;
		case '2':
			b_adjusting_point = true;
			i_which_point_adjusting = 1;
			break;
		case '3':
			b_adjusting_point = true;
			i_which_point_adjusting = 2;
			break;
		case '4':
			b_adjusting_point = true;
			i_which_point_adjusting = 3;
			break;
		case 'x':
			b_adjusting_point = false;
			break;
		case 'q':
			b_start_game = true;
			i_start_time = ofGetElapsedTimeMillis();
			break;
	}
	
}

//--------------------------------------------------------------
void LastStandApplication::keyReleased(int key){
	
}

//--------------------------------------------------------------
void LastStandApplication::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void LastStandApplication::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void LastStandApplication::mousePressed(int x, int y, int button){
	
	// for warping image
	if(b_adjusting_point == true)
	{
		camera.setSourcePoint(i_which_point_adjusting, x, y);
	}
	
	/*********************************************************************************
	 ** THIS WILL TRANSFER TO IRTRACKER CLASS
	 ** Goes through all creatures and checks if mouse has hit one
	 ** TODO: instead of on mouse click this should occur when laser is tracked
	 *********************************************************************************
	 
	 for(int i = 0; i<creatures.size(); i++)
	{
		// A creature knows when it is hit
		bool b_creatureHit = creatures[i].checkHit(float (x), float (y));
		
		// kill creature if hit or show a bullet hole on ground if not hit
		if(b_creatureHit == true )
		{
			//printf("i'm hit\n");
			//creatures[i].kill();
			creatures.erase(creatures.begin()+i);
			
			// update game score with all the info it needs to keep track of stats
			// TODO: add more params to update. it should keep track of info about the creature and player for stats
			//gameScore.update(2);
		}
		else
		{
			// TODO: display a bullet hole sprite 
			//printf("no hit\n");
		}
	}*/
	
}

//--------------------------------------------------------------
void LastStandApplication::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void LastStandApplication::windowResized(int w, int h){
	
}
