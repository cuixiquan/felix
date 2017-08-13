// Copyright 2013 Hazy Research [http://hazy.cs.wisc.edu]
//   University of Wisconsin-Madison [http://www.wisc.edu]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//  Filename: BUGS.h
//  Project:  Elementary_0.2
//
//  Created by Shriram Sridharan on 05/13/13.


#ifndef DBCOMMON_H_
#define DBCOMMON_H_

static void exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

static void executeSQLCommand(PGconn *conn, PGresult* &res, string sql, string errmsg, string successmsg) {
	res = PQexec(conn, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK && errmsg != "")
	{
		cout << errmsg << PQerrorMessage(conn) << endl;
		PQclear(res);
		exit_nicely(conn);
	}

	if(successmsg != "")
		cout << successmsg << endl;
}

PGresult* executeSQLQuery(PGconn *conn, string sql) {
	PGresult *res = PQexec(conn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		cout << "Query Execution Failed with Message : " << PQerrorMessage(conn) << endl;
		PQclear(res);
		exit_nicely(conn);
	}

	return res;
}

#endif /* DBCOMMON_H_ */
