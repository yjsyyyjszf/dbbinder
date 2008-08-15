/*
    Copyright 2008 Gianni Rossi

    This file is part of DBBinder++.

    DBBinder++ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DBBinder++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DBBinder++.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DBBUILDERABSTRACTGENERATOR_H
#define DBBUILDERABSTRACTGENERATOR_H

#include "main.h"

namespace DBBuilder
{

enum SQLTypes
{
	stUnknown,
	stInt,
	stFloat,
	stDouble,
	stTimeStamp,
	stTime,
	stDate,
	stText
};
SQLTypes typeNameToSQLType(String _name);
	
struct SQLElement
{
	SQLElement( const String& _name, SQLTypes _type, int _index = -1 ):
			name( _name ), type( _type ), index( _index )
	{}
	SQLElement( const String& _name, SQLTypes _type, int _index, const String& _default ):
			name( _name ), type( _type ), index( _index ), defaultValue( _default )
	{}

	String		name;
	SQLTypes	type;
	int			index;
	String		defaultValue;
};
typedef std::list<SQLElement> ListElements;

struct AbstractElements
{
	String			name;
	String			sql;
	ListElements	input;
};

struct SelectElements: public AbstractElements
{
	ListElements	output;
};

struct UpdateElements: public AbstractElements
{
};
typedef UpdateElements InsertElements;

class AbstractGenerator
{
	protected:
		AbstractGenerator();
		virtual ~AbstractGenerator();

		typedef std::map<String, String> _dbParams;

		_dbParams	m_dbParams;

		enum _fileTypes
		{
			ftIntf,
			ftImpl,
			ftMAX
		};
		struct _classParams
		{
			SelectElements	select;
			UpdateElements	update;
			InsertElements	insert;
		};
		typedef boost::shared_ptr<_classParams> _classParamsPtr;
		typedef std::map<String, _classParamsPtr> classParams;

		typedef std::map<SQLTypes, String> mapTypes;
		mapTypes	m_types;

		ListString	m_namespaces;
		ListString	m_headers;
		
		String		m_dbengine;
		
		classParams	m_classParams;

		bool m_connected;
		virtual bool checkConnection();

		String	m_outIntFile;
		String	m_outImplFile;
		
		google::TemplateDictionary	*m_dict;
		google::Template			*m_templ[ftMAX];

		virtual void loadDictionary();
		
		virtual void loadDatabase();
		virtual bool loadXMLDatabase(const String& _path);
		virtual bool loadYAMLDatabase(const String& _path);
		
		virtual void loadTemplates();
		virtual bool loadXMLTemplate(const String& _path);
		virtual bool loadYAMLTemplate(const String& _path);

		virtual String getBind(const ListElements::iterator& _item, int _index) = 0;
		virtual String getReadValue(const ListElements::iterator& _item, int _index) = 0;
	public:
		static AbstractGenerator* getGenerator(const String& _type);

		void setDBParam(const String& _key, const String& _value)
		{
			m_dbParams[_key] = _value;
		}

		void setType(SQLTypes _sqlType, const String& _genType)
		{
			m_types[_sqlType] = _genType;
		}
		
		String getType(SQLTypes _sqlType);
		String getInit(SQLTypes _sqlType);

		void addNamespace(const String& _name)
		{
			m_namespaces.push_back(_name);
		}

		void addHeader(const String& _header)
		{
			m_headers.push_back(_header);
		}
		
		virtual void generate();
		
		virtual void addSelect(SelectElements _elements);
		virtual void addUpdate(UpdateElements _elements);
		virtual void addInsert(InsertElements _elements);
};

}

#endif