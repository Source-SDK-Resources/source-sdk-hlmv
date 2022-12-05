//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#ifndef INCLUDED_STUDIOMODEL
#define INCLUDED_STUDIOMODEL

#include "mathlib.h"
#include "studio.h"
#include "mouthinfo.h"
#include "UtlLinkedList.h"
#include "UtlSymbol.h"
#include "bone_setup.h"
#include "datacache/imdlcache.h"

#define DEFAULT_BLEND_TIME 0.2

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
typedef struct IFACE_TAG IFACE;
typedef struct IMESH_TAG IMESH;
typedef struct ResolutionUpdateTag ResolutionUpdate;
typedef struct FaceUpdateTag FaceUpdate;
class IMaterial;
class IDataCache;
class IStudioPhysics;
class IMaterialSystem;
class IMDLCache;
class CPhysmesh;
struct hlmvsolid_t;
struct constraint_ragdollparams_t;
class IStudioRender;
class IPhysicsSurfaceProps;
class IPhysicsCollision;
class IStudioDataCache;
class IDataCache;
class IFileSystem;
class IMaterialSystemHardwareConfig;


//-----------------------------------------------------------------------------
// Singleton interfaces
//-----------------------------------------------------------------------------
extern IStudioRender *g_pStudioRender;
extern IMDLCache *g_pMDLCache;
extern IPhysicsSurfaceProps *physprop;
extern IPhysicsCollision *physcollision;
extern IStudioDataCache *g_pStudioDataCache;
extern IDataCache *g_pDataCache;
extern IFileSystem *g_pFileSystem;
extern IMaterialSystem *g_pMaterialSystem;
extern IMaterialSystemHardwareConfig *g_pMaterialSystemHardwareConfig;


class AnimationLayer
{
public:
	float					m_cycle;			// 0 to 1 animation playback index
	int						m_sequence;			// sequence index
	float					m_weight;
	float					m_playbackrate;
	int						m_priority;			// lower priorities get layered first
};



class StudioModel
{
public:
	StudioModel();

	// memory handling, uses calloc so members are zero'd out on instantiation
    static void						*operator new( size_t stAllocateBlock );
	static void						operator delete( void *pMem );

	static void				Init( void );
	static void				Shutdown( void ); // garymcthack - need to call this.
	static void				UpdateViewState( const Vector& viewOrigin,
											 const Vector& viewRight,
											 const Vector& viewUp,
											 const Vector& viewPlaneNormal );


	static void						ReleaseStudioModel( void );
	static void						RestoreStudioModel( void );

	static void						UnloadGroupFiles();

	char const						*GetFileName( void );

	static IStudioRender			*GetStudioRender();

	static void UpdateStudioRenderConfig( bool bWireframe, bool bZBufferWireframe, bool bNormals, bool bTangentFrame );
	studiohdr_t						*getAnimHeader (int i) const;

	virtual void					ModelInit( void ) { }

	bool							IsModelLoaded() const;

	void							FreeModel( bool bReleasing );
	bool							LoadModel( const char *modelname );
	virtual bool					PostLoadModel ( const char *modelname );
	bool							HasModel();

	virtual void					DrawModel( bool mergeBones = false );

	virtual void					AdvanceFrame( float dt );
	float							GetInterval( void );
	float							GetCycle( void );
	float							GetFrame( void );
	int								GetMaxFrame( void );
	int								SetFrame( int frame );
	float							GetCycle( int iLayer );
	float							GetFrame( int iLayer );
	int								GetMaxFrame( int iLayer );
	int								SetFrame( int iLayer, int frame );

	void							ExtractBbox( Vector &mins, Vector &maxs );

	void							SetBlendTime( float blendtime );
	int								LookupSequence( const char *szSequence );
	int								SetSequence( int iSequence );
	int								SetSequence( const char *szSequence );
	void							ClearOverlaysSequences( void );
	void							ClearAnimationLayers( void );
	int								GetNewAnimationLayer( int iPriority = 0 );

	int								SetOverlaySequence( int iLayer, int iSequence, float flWeight );
	float							SetOverlayRate( int iLayer, float flCycle, float flFrameRate );
	int								GetOverlaySequence( int iLayer );
	float							GetOverlaySequenceWeight( int iLayer );
	void							StartBlending( void );

	float							GetTransitionAmount( void );
	int								GetSequence( void );
	void							GetSequenceInfo( int iSequence, float *pflFrameRate, float *pflGroundSpeed );
	void							GetSequenceInfo( float *pflFrameRate, float *pflGroundSpeed );
	float							GetFPS( int iSequence );
	float							GetFPS( );
	float							GetDuration( int iSequence );
	float							GetDuration( );
	int								GetNumFrames( int iSequence );
	bool							GetSequenceLoops( int iSequence );
	void							GetMovement( float prevCycle[5], Vector &vecPos, QAngle &vecAngles );
	void							GetMovement( int iSequence, float prevCycle, float currCycle, Vector &vecPos, QAngle &vecAngles );
	void							GetSeqAnims( int iSequence, mstudioanimdesc_t *panim[4], float *pweights );
	void							GetSeqAnims( mstudioanimdesc_t *panim[4], float *pweights );
	float							GetGroundSpeed( int iSequence );
	float							GetGroundSpeed( void );
	float							GetCurrentVelocity( void );
	bool							IsHidden( int iSequence );

	float							SetController( int iController, float flValue );

	int								LookupPoseParameter( char const *szName );
	float							SetPoseParameter( int iParameter, float flValue );
	float							SetPoseParameter( char const *szName, float flValue );
	float							GetPoseParameter( char const *szName );
	float							GetPoseParameter( int iParameter );
	bool 							GetPoseParameterRange( int iParameter, float *pflMin, float *pflMax );

	int								LookupAttachment( char const *szName );

	int								SetBodygroup( int iGroup, int iValue );
	int								SetSkin( int iValue );
	int								FindBone( const char *pName );

	LocalFlexController_t           LookupFlexController( const char *szName );
	void							SetFlexController( const char *szName, float flValue );
	void							SetFlexController( LocalFlexController_t iFlex, float flValue );
	float							GetFlexController( const char *szName );
	float							GetFlexController( LocalFlexController_t iFlex );
	void							SetFlexControllerRaw( LocalFlexController_t iFlex, float flValue );
	float							GetFlexControllerRaw( LocalFlexController_t iFlex );

	// void							CalcBoneTransform( int iBone, Vector pos[], Quaternion q[], matrix3x4_t& bonematrix );

	void							UpdateBoneChain( Vector pos[], Quaternion q[], int iBone, matrix3x4_t *pBoneToWorld );
	void							SetViewTarget( void ); // ???
	void							GetBodyPoseParametersFromFlex( void );
	void							SetHeadPosition( Vector pos[], Quaternion q[] );
	float							SetHeadPosition( matrix3x4_t& attToWorld, Vector const &vTargetPos, float dt );

	int								GetNumLODs() const;
	float							GetLODSwitchValue( int lod ) const;
	void							SetLODSwitchValue( int lod, float switchValue );
	
	void							scaleMeshes( float scale );
	void							scaleBones( float scale );

	// Physics
	void							OverrideBones( bool *override );
	int								Physics_GetBoneCount( void );
	const char *					Physics_GetBoneName( int index );
	int								Physics_GetBoneIndex( const char *pName );
	void							Physics_GetData( int boneIndex, hlmvsolid_t *psolid, constraint_ragdollparams_t *pConstraint ) const;
	void							Physics_SetData( int boneIndex, const hlmvsolid_t *psolid, constraint_ragdollparams_t const *pConstraint );
	void							Physics_SetPreview( int previewBone, int axis, float t );
	float							Physics_GetMass( void );
	void							Physics_SetMass( float mass );
	char							*Physics_DumpQC( void );

	float							GetSequenceTime() const { return m_sequencetime; }
	float							GetTimeDelta() const { return m_dt; }

	CStudioHdr						*m_pStudioHdr;
	CStudioHdr						*GetStudioHdr() const;
	studiohdr_t						*GetStudioRenderHdr() const;
	studiohwdata_t					*GetHardwareData( void ) const;

public:
	// entity settings
	QAngle							m_angles;	// rot
	Vector							m_origin;	// trans

protected:
	int								m_bodynum;			// bodypart selection	
	int								m_skinnum;			// skin group selection
	float							m_controller[4];	// bone controllers

public:
	CMouthInfo						m_mouth;

protected:
	char							*m_pModelName;		// model file name

	// bool							m_owntexmodel;		// do we have a modelT.mdl ?

	// Previouse sequence data
	float							m_blendtime;
	float							m_sequencetime;
	int								m_prevsequence;
	float							m_prevcycle;

	float							m_dt;

	// Blending info

	// Gesture,Sequence layering state
#define MAXSTUDIOANIMLAYERS			8
	AnimationLayer					m_Layer[MAXSTUDIOANIMLAYERS];
	int								m_iActiveLayers;

public:
	float							m_cycle;			// 0 to 1 animation playback index
protected:
	int								m_sequence;			// sequence index
	float							m_poseparameter[MAXSTUDIOPOSEPARAM];		// intra-sequence blending
	float							m_weight;

	// internal data
	MDLHandle_t						m_MDLHandle;
	mstudiomodel_t					*m_pmodel;

public:
	// I'm saving this as internal data because we may add or remove hitboxes
	// I'm using a utllinkedlist so hitbox IDs remain constant on add + remove
	typedef CUtlLinkedList<mstudiobbox_t, unsigned short> Hitboxes_t;
	CUtlVector< Hitboxes_t >		m_HitboxSets;
	struct hbsetname_s
	{
		char name[ 128 ];
	};

	CUtlVector< hbsetname_s >		m_HitboxSetNames;

	CUtlVector< CUtlSymbol >		m_SurfaceProps;

protected:
	// class data
	static Vector					*m_AmbientLightColors;

	// Added data
	// IMESH						*m_pimesh;
	// VertexUpdate					*m_pvertupdate;
	// FaceUpdate					*m_pfaceupdate;
	IFACE							*m_pface;

	// studiohdr_t					*m_ptexturehdr;

	Vector4D						m_adj;				// FIX: non persistant, make static

public:
	IStudioPhysics					*m_pPhysics;
private:
	int								m_physPreviewBone;
	int								m_physPreviewAxis;
	float							m_physPreviewParam;
	float							m_physMass;

public:
	mstudioseqdesc_t				&GetSeqDesc( int seq );
private:
	mstudioanimdesc_t				&GetAnimDesc( int anim );
	mstudioanim_t					*GetAnim( int anim );

	void							Transform( Vector const &in1, mstudioboneweight_t const *pboneweight, Vector &out1 );
	void							Rotate( Vector const &in1, mstudioboneweight_t const *pboneweight, Vector &out1 );
	void							DrawPhysmesh( CPhysmesh *pMesh, int boneIndex, IMaterial *pMaterial, float *color );
	void							DrawPhysConvex( CPhysmesh *pMesh, IMaterial *pMaterial );

	void							SetupLighting( void );

	virtual void					SetupModel( int bodypart );

private:
	float							m_flexweight[MAXSTUDIOFLEXCTRL];
public:
	virtual int						FlexVerts( mstudiomesh_t *pmesh );
	virtual void					RunFlexRules( void );
	virtual int						BoneMask( void );
	virtual void					SetUpBones( bool mergeBones );

	const char						*GetKeyValueText( int iSequence );

private:
	// Drawing helper methods
	void DrawBones( );
	void DrawAttachments( );
	void DrawEditAttachment();
	void DrawHitboxes();
	void DrawPhysicsModel( );
	void DrawIllumPosition( );

public:
	// generic interface to rendering?
	static void drawBox (Vector const *v, float const * color );
	static void drawWireframeBox (Vector const *v, float const* color );
	static void drawTransform( matrix3x4_t& m, float flLength = 4 );
	static void drawLine( Vector const &p1, Vector const &p2, int r = 0, int g = 0, int b = 255 );
	static void drawTransparentBox( Vector const &bbmin, Vector const &bbmax, const matrix3x4_t& m, float const *color, float const *wirecolor );



private:
	struct DrawMetrics {
		// Maybe turn this into a DrawModelResults_t at a later date?
		int LodUsed   = 0;
		int LodMetric = 0;
		int PolyCount = 0;
		int NumHardwareBones = 0;
		int NumBatches   = 0;
		int NumMaterials = 0;
	} m_drawMetrics;
public:
	const DrawMetrics& GetDrawMetrics() { return m_drawMetrics; }


public:

	void					SetSolveHeadTurn( int solve );
	int						GetSolveHeadTurn() const;

	//void					SetHeadTurn( float turn );
	//float					GetHeadTurn() const;
	void					ClearHeadTarget( void );

	void					SetHeadTarget( const Vector& target, float turn );
	void					GetHeadTarget( Vector& target ) const;

	void					SetEyeTarget( const Vector& target );
	void					GetEyeTarget( Vector& target ) const;

	void					SetHasLookTarget( bool hastarget );
	bool					GetHasLookTarget() const;

	void					SetModelYaw( float yaw );
	float					GetModelYaw( void ) const;
	void					SetBodyYaw( float yaw );
	float					GetBodyYaw( void ) const;
	void					SetSpineYaw( float yaw );
	float					GetSpineYaw( void ) const;

private:

	// 0 == no, 1 == based on dt, 2 == completely.
	int						m_nSolveHeadTurn; 
	float					m_flHeadTurn;
	Vector					m_vecHeadTarget;
	Vector					m_vecEyeTarget;
	bool					m_bHasLookTarget;

	float					m_flModelYaw;
	float					m_flBodyYaw;
	float					m_flSpineYaw;

public:
	bool					m_bIsTransparent;
	bool					m_bHasProxy;

	// necessary for accessing correct vertexes
	void					SetCurrentModel();

public:
	CIKContext				m_ik;
	float					m_prevGroundCycles[5];
	float					m_prevIKCycles[5];

public:
	void					IncrementFramecounter( void ) { m_iFramecounter++; };
private:
	int						m_iFramecounter;
};


//-----------------------------------------------------------------------------
// Inline methods 
//-----------------------------------------------------------------------------
inline CStudioHdr *StudioModel::GetStudioHdr( void ) const 
{ 
	if (!m_pStudioHdr || m_pStudioHdr->IsReadyForAccess())
		return m_pStudioHdr;

	studiohdr_t *hdr = g_pMDLCache->GetStudioHdr( m_MDLHandle );

	m_pStudioHdr->Init( hdr );

	if (m_pStudioHdr->IsReadyForAccess())
		return m_pStudioHdr;

	return NULL;
}

inline studiohdr_t *StudioModel::GetStudioRenderHdr( void ) const 
{
	return g_pMDLCache->GetStudioHdr( m_MDLHandle );
}

inline studiohwdata_t *StudioModel::GetHardwareData( void ) const 
{ 
	return g_pMDLCache->GetHardwareData( m_MDLHandle );
}

inline studiohdr_t *StudioModel::getAnimHeader( int i ) const 
{ 
//	return g_pMDLCache->GetStudioHdr( m_AnimHandle[i] );
//	return m_panimhdr[i]; 
}

inline char const *StudioModel::GetFileName( void ) 
{ 
	return m_pModelName; 
}

inline IStudioRender *StudioModel::GetStudioRender() 
{ 
	return g_pStudioRender; 
}

inline bool StudioModel::IsModelLoaded() const 
{ 
	return m_MDLHandle != MDLHANDLE_INVALID; 
}


//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
extern Vector g_vright;		// needs to be set to viewer's right in order for chrome to work
extern StudioModel *g_pStudioModel;
extern StudioModel *g_pStudioExtraModel[4];


#endif // INCLUDED_STUDIOMODEL
