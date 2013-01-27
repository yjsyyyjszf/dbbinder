//
// C++ Interface: insert.h
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __INCLUDE_INSERT_H
#define __INCLUDE_INSERT_H

#if __cplusplus < 201103L
#include <boost/shared_ptr.hpp>
#else
#include <memory>
#endif

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <iostream>
#include <string.h>
#include <libgen.h>

#include <mysql/mysql.h>
#include <mysql/errmsg.h>

using boost::posix_time::ptime;

#ifdef NDEBUG
#ifndef ASSERT_MSG
#define ASSERT_MSG(cond, msg) do { if (!(cond)) { std::cerr << " WARNING: " << msg << std::endl; assert(cond); }} while (false)
#endif
#ifndef LOG_MSG
#define LOG_MSG(msg) { std::cerr << " WARNING: " << msg << std::endl; } while (false)
#endif
#else
#ifndef ASSERT_MSG
#define ASSERT_MSG(cond, msg) do { if (!(cond)) { std::cerr << __FILE__ << ':' << __LINE__ << " WARNING: " << msg << std::endl; assert(cond); }} while (false)
#endif
#ifndef LOG_MSG
#define LOG_MSG(msg) do { std::cerr << __FILE__ << ':' << __LINE__ << " WARNING: " << msg << std::endl; } while (false)
#endif
#endif

#ifndef DBBINDERCONVERTTIME
#define DBBINDERCONVERTTIME
inline MYSQL_TIME dbbinderConvertTime(ptime v)
{
    MYSQL_TIME result;

    if (v.is_not_a_date_time())
    {
        memset(&result, 0, sizeof(MYSQL_TIME));
    }
    else
    {
        const boost::gregorian::date& d = v.date();
        result.year = d.year();
        result.month = d.month();
        result.day = d.day();

        const boost::posix_time::time_duration& t = v.time_of_day();
        result.hour = t.hours();
        result.minute = t.minutes();
        result.second = t.seconds();

        result.neg = false;
        result.time_type = MYSQL_TIMESTAMP_DATETIME;
    }
    return result;
}

inline ptime dbbinderConvertTime(MYSQL_TIME v)
{
    if (v.year && v.month && v.day)
        return ptime(boost::gregorian::date(v.year, v.month, v.day),
                     boost::posix_time::time_duration(v.hour, v.minute, v.second, v.second_part));
    return ptime();
}
#endif // DBBINDER_CONVERT_TIME_MYSQL_TIME

#ifndef MYSQLCHECKSTMTERR
#define MYSQLCHECKSTMTERR
inline void mysqlCheckStmtErr(MYSQL_STMT *_stmt, int _status)
{
    if (_status)
    {
        switch(_status)
        {
        case CR_COMMANDS_OUT_OF_SYNC:
            ASSERT_MSG(false, "Commands were executed in an improper order.");
            break;
        case CR_OUT_OF_MEMORY:
            ASSERT_MSG(false, "Out of memory.");
            break;
        case CR_SERVER_GONE_ERROR:
            ASSERT_MSG(false, "The MySQL server has gone away.");
            break;
        case CR_SERVER_LOST:
            ASSERT_MSG(false, "The m_connection to the server was lost during the query.");
            break;
        case CR_UNKNOWN_ERROR:
            ASSERT_MSG(false, "An unknown error occurred.");
            break;
        case CR_UNSUPPORTED_PARAM_TYPE:
            ASSERT_MSG(false, "The buffer type is MYSQL_TYPE_DATE, MYSQL_TYPE_TIME, MYSQL_TYPE_DATETIME, or MYSQL_TYPE_TIMESTAMP, but the data type is not DATE, TIME, DATETIME, or TIMESTAMP.");
            break;
        }

        ASSERT_MSG(false, "MySQL: " << mysql_stmt_error(_stmt));
        exit(-1);
    }
}
#endif // MYSQLCHECKSTMTERR

/**
 * @class insert_blob
 * @brief File autogenerated by dbbinder++
 * This class encapsulates:
 *
 *
 *
 * @param comment std::shared_ptr< std::vector<char> >
 *
 * insert into
    dbtest (`comment`)
values
    (?)

 *
 */
class insert_blob
{
public:
    insert_blob(MYSQL* _conn);
    ~insert_blob();
private:
    MYSQL*	m_conn;

private:
    static const char* const s_insertSQL;
    static const int s_insertSQL_len;
    static const int s_insertParamCount;
    MYSQL_STMT*	m_insertStmt;
public:
    void insert(std::shared_ptr< std::vector<char> > _comment);

};

#undef ASSERT_MSG
#undef LOG_MSG

#endif
