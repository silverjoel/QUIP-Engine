/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#ifndef __GAME_AFENTITY_H__
#define __GAME_AFENTITY_H__

#include "Light.h"	// ################## SR

/*
===============================================================================

idMultiModelAF

Entity using multiple separate visual models animated with a single
articulated figure. Only used for debugging!

===============================================================================
*/
const int GIB_DELAY = 200;  // only gib this often to keep performace hits when blowing up several mobs

class idMultiModelAF : public idEntity
{
public:
	CLASS_PROTOTYPE( idMultiModelAF );
	
	void					Spawn();
	~idMultiModelAF();
	
	virtual void			Think();
	virtual void			Present();
	
protected:
	idPhysics_AF			physicsObj;
	
	void					SetModelForId( int id, const idStr& modelName );
	
private:
	idList<idRenderModel*, TAG_AF>	modelHandles;
	idList<int, TAG_AF>				modelDefHandles;
};


/*
===============================================================================

idChain

Chain hanging down from the ceiling. Only used for debugging!

===============================================================================
*/

class idChain : public idMultiModelAF
{
public:
	CLASS_PROTOTYPE( idChain );
	
	void					Spawn();
	
protected:
	void					BuildChain( const idStr& name, const idVec3& origin, float linkLength, float linkWidth, float density, int numLinks, bool bindToWorld = true );
};


/*
===============================================================================

idAFAttachment

===============================================================================
*/

class idAFAttachment : public idAnimatedEntity
{
public:
	CLASS_PROTOTYPE( idAFAttachment );
	
	idAFAttachment();
	virtual					~idAFAttachment();
	
	void					Spawn();
	
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
	void					SetBody( idEntity* bodyEnt, const char* headModel, jointHandle_t attachJoint );
	void					ClearBody();
	idEntity* 				GetBody() const;
	
	virtual void			Think();
	
	virtual void			Hide();
	virtual void			Show();
	
	void					PlayIdleAnim( int blendTime );
	
	virtual void			GetImpactInfo( idEntity* ent, int id, const idVec3& point, impactInfo_t* info );
	virtual void			ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse );
	virtual void			AddForce( idEntity* ent, int id, const idVec3& point, const idVec3& force );
	
	virtual	void			Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );
	virtual void			AddDamageEffect( const trace_t& collision, const idVec3& velocity, const char* damageDefName );
	
	void					SetCombatModel();
	idClipModel* 			GetCombatModel() const;
	virtual void			LinkCombat();
	virtual void			UnlinkCombat();
	
protected:
	idEntity* 				body;
	idClipModel* 			combatModel;	// render model for hit detection of head
	int						idleAnim;
	jointHandle_t			attachJoint;
};


/*
===============================================================================

idAFEntity_Base

===============================================================================
*/

class idAFEntity_Base : public idAnimatedEntity
{
public:
	CLASS_PROTOTYPE( idAFEntity_Base );
	
	idAFEntity_Base();
	virtual					~idAFEntity_Base();
	
	void					Spawn();
	
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
	//ivan start
	//recreate dynamically-added constraints to 'constraints' while physics obejct is being restored
	virtual void			RecreateDynamicConstraints( idList<idAFConstraint*, TAG_IDLIB_LIST_PHYSICS> *constraints ); // ##  TAG_IDLIB_LIST_PHYSICS
	//ivan end	
	
	virtual void			Think();
	virtual void			AddDamageEffect( const trace_t& collision, const idVec3& velocity, const char* damageDefName );
	virtual void			GetImpactInfo( idEntity* ent, int id, const idVec3& point, impactInfo_t* info );
	virtual void			ApplyImpulse( idEntity* ent, int id, const idVec3& point, const idVec3& impulse );
	virtual void			AddForce( idEntity* ent, int id, const idVec3& point, const idVec3& force );
	virtual bool			Collide( const trace_t& collision, const idVec3& velocity );
	virtual bool			GetPhysicsToVisualTransform( idVec3& origin, idMat3& axis );
	virtual bool			UpdateAnimationControllers();
	virtual void			FreeModelDef();
	
	virtual bool			LoadAF();
	bool					IsActiveAF() const
	{
		return af.IsActive();
	}
	const char* 			GetAFName() const
	{
		return af.GetName();
	}
	idPhysics_AF* 			GetAFPhysics()
	{
		return af.GetPhysics();
	}
	
	void					SetCombatModel();
	idClipModel* 			GetCombatModel() const;
	// contents of combatModel can be set to 0 or re-enabled (mp)
	void					SetCombatContents( bool enable );
	virtual void			LinkCombat();
	virtual void			UnlinkCombat();
	
	int						BodyForClipModelId( int id ) const;
	
	void					SaveState( idDict& args ) const;
	void					LoadState( const idDict& args );
	
	void					AddBindConstraints();
	void					RemoveBindConstraints();
	
	virtual void			ShowEditingDialog();
	
	static void				DropAFs( idEntity* ent, const char* type, idList<idEntity*>* list );
	
protected:
	idAF					af;				// articulated figure
	idClipModel* 			combatModel;	// render model for hit detection
	int						combatModelContents;
	idVec3					spawnOrigin;	// spawn origin
	idMat3					spawnAxis;		// rotation axis used when spawned
	int						nextSoundTime;	// next time this can make a sound
	
	void					Event_SetConstraintPosition( const char* name, const idVec3& pos );
};

/*
===============================================================================

idAFEntity_Gibbable

===============================================================================
*/

extern const idEventDef		EV_Gib;
extern const idEventDef		EV_Gibbed;

class idAFEntity_Gibbable : public idAFEntity_Base
{
public:
	CLASS_PROTOTYPE( idAFEntity_Gibbable );
	
	idAFEntity_Gibbable();
	~idAFEntity_Gibbable();
	
	void					Spawn();
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	virtual void			Present();
	virtual	void			Damage( idEntity* inflictor, idEntity* attacker, const idVec3& dir, const char* damageDefName, const float damageScale, const int location );
	void					SetThrown( bool isThrown );
	virtual bool			Collide( const trace_t& collision, const idVec3& velocity );
	virtual void			SpawnGibs( const idVec3& dir, const char* damageDefName );
	
	bool					IsGibbed()
	{
		return gibbed;
	};
	
protected:
	idRenderModel* 			skeletonModel;
	int						skeletonModelDefHandle;
	bool					gibbed;
	
	bool					wasThrown;
	
	virtual void			Gib( const idVec3& dir, const char* damageDefName );
	void					InitSkeletonModel();
	
	void					Event_Gib( const char* damageDefName );
};

/*
===============================================================================

	idAFEntity_Generic

===============================================================================
*/

class idAFEntity_Generic : public idAFEntity_Gibbable
{
public:
	CLASS_PROTOTYPE( idAFEntity_Generic );
	
	idAFEntity_Generic();
	~idAFEntity_Generic();
	
	void					Spawn();
	
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
	virtual void			Think();
	void					KeepRunningPhysics()
	{
		keepRunningPhysics = true;
	}
	
private:
	void					Event_Activate( idEntity* activator );
	
	bool					keepRunningPhysics;
};


/*
===============================================================================

idAFEntity_WithAttachedHead

===============================================================================
*/

class idAFEntity_WithAttachedHead : public idAFEntity_Gibbable
{
public:
	CLASS_PROTOTYPE( idAFEntity_WithAttachedHead );
	
	idAFEntity_WithAttachedHead();
	~idAFEntity_WithAttachedHead();
	
	void					Spawn();
	
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
	void					SetupHead();
	
	virtual void			Think();
	
	virtual void			Hide();
	virtual void			Show();
	virtual void			ProjectOverlay( const idVec3& origin, const idVec3& dir, float size, const char* material );
	
	virtual void			LinkCombat();
	virtual void			UnlinkCombat();
	
protected:
	virtual void			Gib( const idVec3& dir, const char* damageDefName );
	
public:
	idEntityPtr<idAFAttachment>	head;
	
	void					Event_Gib( const char* damageDefName );
	void					Event_Activate( idEntity* activator );
};


/*
===============================================================================

idAFEntity_Vehicle

===============================================================================
*/

class idAFEntity_Vehicle : public idAFEntity_Base
{
public:
	CLASS_PROTOTYPE( idAFEntity_Vehicle );
	
	idAFEntity_Vehicle();
	
	void					Spawn();
	void					Use( idPlayer* player );
		
	// ########################################################## SR	

	//virtual void			Init();
	
	void 					HitObject( void );	// const trace_t &collision, const idVec3 &velocity  );
	void					FireBullet( void );	
	void					FireBomb( void );	
	void 					Aim( void );							
	void					Save( idSaveGame *savefile ) const;	
	void					Restore( idRestoreGame *savefile );	
	int						GetZoomFov( void );
	void 					LightOnOff( bool on );
	bool					lightOn;

	jointHandle_t			aimJoint;	
	jointHandle_t			turnJoint;	
	jointHandle_t			fireJoint;
	jointHandle_t			barrelJoint;	
	
	jointHandle_t			eyesJoint;
	jointHandle_t			steeringWheelJoint;
	
	jointHandle_t			headlightJoint;	
	jointHandle_t			exhaustJoint1;	
	jointHandle_t			exhaustJoint2;
	jointHandle_t			exhaustJoint3;
	jointHandle_t			exhaustJoint4;
	
	// motorsep 03-07-2015; a flag to let engine know that if vehicle's .def has no exhaust joints (if model has none) or smoke particle effect
	// don't bother with it and work without exhaust effect
	bool 					noExhaust;			// = false; NB You can't initialize a bool in the class declaration

	idEntityPtr<idPlayer>	player;
	
	// ####################################################### SR END
	
protected:

// ############################################################# SR	

	bool					netSyncPhysics; 
	bool					isAccelerating; 
	bool					isDecelerating; 
	
	void 					TireTrack( const idVec3 &origin, float angle, const idMaterial* material );
	void 					LaunchProjectile( float spread, const char *projSound );	
	
	int						zoomFov;	
	int 					accelTime;
	
	idDict					brassDict;
	int						brassDelay;
	jointHandle_t			ejectJointView;
	
	idMat3					muzzleAxis;
	idVec3					muzzleOrigin;
	idVec3					oldOrigin;
	idPhysics_AF				physicsObj;
	idDict 					projectileDict;
	idDict 					bulletDict;
	idDict 					bombDict;
	
	idEntityPtr<idLight>	headlight;
	idEntityPtr<idLight>	headlighta;

	idRotation 				barrelRotation;
	const idDeclParticle *	dustSmoke2;
	const idDeclParticle *	gunSmoke;
	const idDeclParticle *	exhaustSmoke;
	const idDeclEntityDef *	vehicleDef;		
	float					fireTime;
	float 					fireDelay;
	float					nextTrackTime;
	float 					maxSteerAngle;
	float 					currentBrakes;
	
	float					vehicleVelocity;
	float					vehicleForce;
	float					vehicleSuspensionUp;
	float					vehicleSuspensionDown;
	float					vehicleSuspensionKCompress;
	float					vehicleSuspensionDamping;
	float					vehicleTireFriction;	
	
	const idMaterial* 		trackDecal;
	
	
// ######################################################## SR END		

	//idPlayer* 				player;
	//jointHandle_t			eyesJoint;
	//jointHandle_t			steeringWheelJoint;
	
	float					wheelRadius;
	float					steerAngle;
	float					steerSpeed;
	const idDeclParticle* 	dustSmoke;
	
	// shared auto drive functionality
	idEntityPtr<idEntity>	autoDriveWaypoint;
	float					autoDriveSteerSpeed;
	float					autoDriveIdealSteer;
	float					autoDriveSpeed;
	float					autoDriveReachTolerance;

	float					GetAutoDriveSpeed() const;

	float					GetCurrentSteerAngle() const;
	float					GetIdealSteerAngle() const;

	void					UpdateSteerAngle();

	void					Event_HeadLightsOn( int on );
	void					Event_GetAutoDriveWaypoint();
	void					Event_SetAutoDriveWaypoint( idEntity * entity );
	void					Event_SetAutoDriveSteerSpeed( float );

	void					PostSpawn();
};


/*
===============================================================================

idAFEntity_VehicleSimple

===============================================================================
*/

class idAFEntity_VehicleSimple : public idAFEntity_Vehicle
{
public:
	CLASS_PROTOTYPE( idAFEntity_VehicleSimple );
	
	idAFEntity_VehicleSimple();
	~idAFEntity_VehicleSimple();
	
	void					Spawn();
	virtual void			Think();
	
protected:
	idClipModel* 			wheelModel;
	idAFConstraint_Suspension* 	suspension[4];
	jointHandle_t			wheelJoints[4];
	float					wheelAngles[4];
};

/*
===============================================================================

idAFEntity_VehicleSimple_4wd

===============================================================================
*/

class idAFEntity_VehicleSimple_4wd : public idAFEntity_Vehicle {
public:
	CLASS_PROTOTYPE( idAFEntity_VehicleSimple_4wd );

							idAFEntity_VehicleSimple_4wd( void );
							~idAFEntity_VehicleSimple_4wd( void );

	void					Spawn( void );
	virtual void			Think( void );
	
	// ############### SR
	virtual void			WriteToSnapshot( idBitMsg & msg ) const;
	virtual void			ReadFromSnapshot( const idBitMsg& msg );	
	virtual void			ClientPredictionThink( void );
	float					bvelocity;
	float					bforce;
	// ############### END	

	//ivan start
	void					Save( idSaveGame *savefile ) const;
	void					Restore( idRestoreGame *savefile );
	virtual void			RecreateDynamicConstraints( idList<idAFConstraint*, TAG_IDLIB_LIST_PHYSICS> *constraints ); // ##  TAG_IDLIB_LIST_PHYSICS
	//ivan end

	bool					Pain( idEntity* inflictor, idEntity* attacker, int damage, const idVec3& dir, int location );
protected:
	//idAFBody *				wheels[4];
	idClipModel *				wheelModel;
	idAFConstraint_Suspension *	suspension[4];
	jointHandle_t				wheelJoints[4];
	float						wheelAngles[4];
	int						wheelConstraints[4];
	
	float						tempAngles[4];	// #### SR
	idVec3 						wheelorigin[4];	// #### SR
	
	virtual bool			Collide( const trace_t &collision, const idVec3 &velocity ); // #########  SR
	
};


/*
===============================================================================

idAFEntity_VehicleFourWheels

===============================================================================
*/

class idAFEntity_VehicleFourWheels : public idAFEntity_Vehicle
{
public:
	CLASS_PROTOTYPE( idAFEntity_VehicleFourWheels );
	
	idAFEntity_VehicleFourWheels();
	
	void					Spawn();
	virtual void			Think();

// motorsep 01-18-2015; moved these variables into header from cpp

	float					force;
	float					velocity;
	float					steerAngle;
	
protected:
	idAFBody* 				wheels[4];
	idAFConstraint_Hinge* 	steering[2];
	jointHandle_t			wheelJoints[4];
	float					wheelAngles[4];
};


/*
===============================================================================

idAFEntity_VehicleSixWheels

===============================================================================
*/

class idAFEntity_VehicleSixWheels : public idAFEntity_Vehicle
{
public:
	CLASS_PROTOTYPE( idAFEntity_VehicleSixWheels );
	
	idAFEntity_VehicleSixWheels();
	
	void					Spawn();
	virtual void			Think();
	
	float					force;
	float					velocity;
	float					steerAngle;
	
private:
	idAFBody* 				wheels[6];
	idAFConstraint_Hinge* 	steering[4];
	jointHandle_t			wheelJoints[6];
	float					wheelAngles[6];
};

/*
===============================================================================

idAFEntity_SteamPipe

===============================================================================
*/

class idAFEntity_SteamPipe : public idAFEntity_Base
{
public:
	CLASS_PROTOTYPE( idAFEntity_SteamPipe );
	
	idAFEntity_SteamPipe();
	~idAFEntity_SteamPipe();
	
	void					Spawn();
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
	virtual void			Think();
	
private:
	int						steamBody;
	float					steamForce;
	float					steamUpForce;
	idForce_Constant		force;
	renderEntity_t			steamRenderEntity;
	qhandle_t				steamModelDefHandle;
	
	void					InitSteamRenderEntity();
};


/*
===============================================================================

idAFEntity_ClawFourFingers

===============================================================================
*/

class idAFEntity_ClawFourFingers : public idAFEntity_Base
{
public:
	CLASS_PROTOTYPE( idAFEntity_ClawFourFingers );
	
	idAFEntity_ClawFourFingers();
	
	void					Spawn();
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
private:
	idAFConstraint_Hinge* 	fingers[4];
	
	void					Event_SetFingerAngle( float angle );
	void					Event_StopFingers();
};


/**
* idHarvestable contains all of the code required to turn an entity into a harvestable
* entity. The entity must create an instance of this class and call the appropriate
* interface methods at the correct time.
*/
class idHarvestable : public idEntity
{
public:
	CLASS_PROTOTYPE( idHarvestable );
	
	idHarvestable();
	~idHarvestable();
	
	void				Spawn();
	void				Init( idEntity* parent );
	void				Save( idSaveGame* savefile ) const;
	void				Restore( idRestoreGame* savefile );
	
	void				SetParent( idEntity* parent );
	
	void				Think();
	void				Gib();
	
protected:
	idEntityPtr<idEntity>	parentEnt;
	float					triggersize;
	idClipModel* 			trigger;
	float					giveDelay;
	float					removeDelay;
	bool					given;
	
	idEntityPtr<idPlayer>	player;
	int						startTime;
	
	bool					fxFollowPlayer;
	idEntityPtr<idEntityFx>	fx;
	idStr					fxOrient;
	
protected:
	void					BeginBurn();
	void					BeginFX();
	void					CalcTriggerBounds( float size, idBounds& bounds );
	
	bool					GetFxOrientationAxis( idMat3& mat );
	
	void					Event_SpawnHarvestTrigger();
	void					Event_Touch( idEntity* other, trace_t* trace );
} ;


/*
===============================================================================

idAFEntity_Harvest

===============================================================================
*/



class idAFEntity_Harvest : public idAFEntity_WithAttachedHead
{
public:
	CLASS_PROTOTYPE( idAFEntity_Harvest );
	
	idAFEntity_Harvest();
	~idAFEntity_Harvest();
	
	void					Spawn();
	
	void					Save( idSaveGame* savefile ) const;
	void					Restore( idRestoreGame* savefile );
	
	virtual void			Think();
	
	virtual void			Gib( const idVec3& dir, const char* damageDefName );
	
protected:
	idEntityPtr<idHarvestable>	harvestEnt;
protected:
	void					Event_SpawnHarvestEntity();
	
};

#endif /* !__GAME_AFENTITY_H__ */
