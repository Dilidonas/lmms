/*
 * dynamics_processor_controls.cpp - controls for dynamics_processor-effect
 *
 * Copyright (c) 2014 Vesa Kivimäki <contact/dot/diizy/at/nbl/dot/fi>
 * Copyright (c) 2008 Tobias Doerffel <tobydox/at/users.sourceforge.net>
 *
 * This file is part of Linux MultiMedia Studio - http://lmms.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */


#include <QtXml/QDomElement>

#include "dynamics_processor_controls.h"
#include "dynamics_processor.h"
#include "graph.h"
#include "engine.h"
#include "song.h"


#define onedB 1.1220184543019633f

dynProcControls::dynProcControls( dynProcEffect * _eff ) :
	EffectControls( _eff ),
	m_effect( _eff ),
	m_inputModel( 1.0f, 0.0f, 5.0f, 0.01f, this, tr( "Input gain" ) ),
	m_outputModel( 1.0f, 0.0f, 5.0f, 0.01f, this, tr( "Output gain" ) ),
	m_attackModel( 10.0f, 1.0f, 500.0f, 1.0f, this, tr( "Attack time" ) ),
	m_releaseModel( 100.0f, 1.0f, 500.0f, 1.0f, this, tr( "Release time" ) ),
	m_wavegraphModel( 0.0f, 1.0f, 200, this ),
	m_stereomodeModel( 0, 0, 2, this, tr( "Stereo mode" ) )
{
	connect( &m_inputModel, SIGNAL( dataChanged() ),
			this, SLOT( changeControl() ) );

	connect( &m_outputModel, SIGNAL( dataChanged() ),
			this, SLOT( changeControl() ) );
			
	connect( &m_attackModel, SIGNAL( dataChanged() ),
			this, SLOT( changeControl() ) );
			
	connect( &m_releaseModel, SIGNAL( dataChanged() ),
			this, SLOT( changeControl() ) );
			
	connect( &m_stereomodeModel, SIGNAL( dataChanged() ),
			this, SLOT( changeControl() ) );

	connect( &m_wavegraphModel, SIGNAL( samplesChanged( int, int ) ),
			this, SLOT( samplesChanged( int, int ) ) );


	setDefaultShape();

}



void dynProcControls::changeControl()
{
	engine::getSong()->setModified();
}

void dynProcControls::samplesChanged( int _begin, int _end)
{
	engine::getSong()->setModified();
}




void dynProcControls::loadSettings( const QDomElement & _this )
{
//load knobs, stereomode
	m_inputModel.setValue( _this.attribute( "inputGain" ).toFloat() );
	m_outputModel.setValue( _this.attribute( "outputGain" ).toFloat() );
	m_attackModel.setValue( _this.attribute( "attack" ).toFloat() );
	m_releaseModel.setValue( _this.attribute( "release" ).toFloat() );
	m_stereomodeModel.setValue( _this.attribute( "stereoMode" ).toInt() );
	
//load waveshape
	int size = 0;
	char * dst = 0;
	base64::decode( _this.attribute( "waveShape"), &dst, &size );

	m_wavegraphModel.setSamples( (float*) dst );
	delete[] dst;

}




void dynProcControls::saveSettings( QDomDocument & _doc,
							QDomElement & _this )
{
//save input, output knobs
	_this.setAttribute( "inputGain", m_inputModel.value() );
	_this.setAttribute( "outputGain", m_outputModel.value() );
	_this.setAttribute( "attack", m_attackModel.value() );
	_this.setAttribute( "release", m_releaseModel.value() );
	_this.setAttribute( "stereoMode", m_stereomodeModel.value() );
	

//save waveshape
	QString sampleString;
	base64::encode( (const char *)m_wavegraphModel.samples(),
		m_wavegraphModel.length() * sizeof(float), sampleString );
	_this.setAttribute( "waveShape", sampleString );

}


void dynProcControls::setDefaultShape()
{
	float shp [200] = { };
	for ( int i = 0; i<200; i++)
	{
		shp[i] = ((float)i + 1.0f) / 200.0f;
	}

	m_wavegraphModel.setLength( 200 );
	m_wavegraphModel.setSamples( (float*)&shp );
}

void dynProcControls::resetClicked()
{
	setDefaultShape();
	engine::getSong()->setModified();
}

void dynProcControls::smoothClicked()
{
	m_wavegraphModel.smoothNonCyclic();
	engine::getSong()->setModified();
}

void dynProcControls::addOneClicked()
{
	for( int i=0; i<200; i++ )
	{
		m_wavegraphModel.setSampleAt( i, qBound( 0.0f, m_wavegraphModel.samples()[i] * onedB, 1.0f ) );
	}
	engine::getSong()->setModified();
}

void dynProcControls::subOneClicked()
{
	for( int i=0; i<200; i++ )
	{
		m_wavegraphModel.setSampleAt( i, qBound( 0.0f, m_wavegraphModel.samples()[i] / onedB, 1.0f ) );
	}
	engine::getSong()->setModified();
}


#include "moc_dynamics_processor_controls.cxx"

