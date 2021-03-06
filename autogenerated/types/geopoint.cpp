// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "geopoint.h"


GeoPoint::GeoPoint(QObject* parent) : TelegramObject(parent)
{
	this->_longitude = 0;
	this->_latitude = 0;
}

void GeoPoint::read(MTProtoStream* mtstream) 
{
	this->_constructorid = mtstream->readTLConstructor();
	
	Q_ASSERT((this->_constructorid == GeoPoint::CtorGeoPointEmpty) ||
		 (this->_constructorid == GeoPoint::CtorGeoPoint));
	
	if(this->_constructorid == GeoPoint::CtorGeoPoint)
	{
		this->_longitude = mtstream->readTLDouble();
		this->_latitude = mtstream->readTLDouble();
	}
}

void GeoPoint::write(MTProtoStream* mtstream) 
{
	Q_ASSERT((this->_constructorid == GeoPoint::CtorGeoPointEmpty) ||
		 (this->_constructorid == GeoPoint::CtorGeoPoint));
	
	this->compileFlags();
	mtstream->writeTLConstructor(this->_constructorid);
	
	if(this->_constructorid == GeoPoint::CtorGeoPoint)
	{
		mtstream->writeTLDouble(this->_longitude);
		mtstream->writeTLDouble(this->_latitude);
	}
}

void GeoPoint::compileFlags() 
{
	
}

TLDouble GeoPoint::longitude() const
{
	return this->_longitude;
}

void GeoPoint::setLongitude(TLDouble longitude) 
{
	if(this->_longitude == longitude)
		return;

	this->_longitude = longitude;
	emit longitudeChanged();
}

TLDouble GeoPoint::latitude() const
{
	return this->_latitude;
}

void GeoPoint::setLatitude(TLDouble latitude) 
{
	if(this->_latitude == latitude)
		return;

	this->_latitude = latitude;
	emit latitudeChanged();
}

