//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef ATTACHMENTS_WINDOW_H
#define ATTACHMENTS_WINDOW_H
#ifdef _WIN32
#pragma once
#endif


#ifndef INCLUDED_MXWINDOW
#include <mx/mxWindow.h>
#endif
#include <mx/mx.h>
#include "mxLineEdit2.h"
#include "vector.h"


class ControlPanel;


class CAttachmentsWindow : public mxWindow
{
public:
	CAttachmentsWindow( ControlPanel* pParent );
	void Init( );

	void OnLoadModel();
	
	void OnTabSelected();
	void OnTabUnselected();
	
	virtual int handleEvent( mxEvent *event );


private:

	void OnSelChangeAttachmentList();

	void PopulateAttachmentsList();
	void PopulateBoneList();
	void UpdateStrings( bool bUpdateQC=true, bool bUpdateTranslation=true, bool bUpdateRotation=true );

	Vector GetCurrentTranslation();
	Vector GetCurrentRotation();


private:

	ControlPanel *m_pControlPanel;
	mxListBox *m_cAttachmentList;
	mxListBox *m_cBoneList;
	
	mxLineEdit2 *m_cTranslation;
	mxLineEdit2 *m_cRotation;
	mxLineEdit2 *m_cQCString;
};


#endif // ATTACHMENTS_WINDOW_H
