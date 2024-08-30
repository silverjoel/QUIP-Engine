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
#pragma hdrstop
#include "precompiled.h"
#include "../Game_local.h"




/*
========================
idMenuScreen_Cinematic::Initialize
========================
*/
void idMenuScreen_Cinematic::Initialize( idMenuHandler* data )
{
	idMenuScreen::Initialize( data );
}

/*
========================
idMenuScreen_Cinematic::ShowScreen
========================
*/
void idMenuScreen_Cinematic::ShowScreen( const mainMenuTransition_t transitionType )
{
	if( menuData != NULL )
	{
		menuGUI = menuData->GetGUI();
	}
	
	if( menuGUI == NULL )
	{
		return;
	}
	
	idSWFScriptObject& root = menuGUI->GetRootObject();
	
	cinematicHint = root.GetNestedSprite( "_bottomCenter", "cinematicHint" );
	
}

/*
========================
idMenuScreen_Cinematic::HideScreen
========================
*/
void idMenuScreen_Cinematic::HideScreen( const mainMenuTransition_t transitionType )
{

}

/*
========================
idMenuScreen_Cinematic::Update
========================
*/
void idMenuScreen_Cinematic::Update()
{

	idPlayer* player = gameLocal.GetLocalPlayer();
	if( player == NULL )
	{
		return;
	}
	
	idMenuScreen::Update();
}


/*
========================
idMenuScreen_Cinematic::ShowCinematicHint
========================
*/
void idMenuScreen_Cinematic::ShowCinematicHint() {
	if ( cinematicHint ) {
		idSWFTextInstance* txtHint = cinematicHint->GetScriptObject()->GetNestedText( "info", "txtHint" );
		if( txtHint != NULL ) {
			idStr hintText = idLocalization::GetString( "#str_press" );
			keyBindings_t bind = idKeyInput::KeyBindingsFromBinding( "_moveUP", true );
			if( !bind.mouse.IsEmpty() ) {
				hintText.Append( " [" );
				hintText.Append( bind.mouse );
				hintText.Append( "] " );
			} else if( !bind.keyboard.IsEmpty() ) {
				hintText.Append( " [" );
				hintText.Append( bind.keyboard );
				hintText.Append( "] " );
			}
			idStr tmptxt  = idLocalization::GetString( "#str_to_skip" );
			hintText.Append( tmptxt );
			txtHint->SetText( hintText );
			txtHint->SetStrokeInfo( true, 0.75f, 1.5f );
		}
	}
}	


