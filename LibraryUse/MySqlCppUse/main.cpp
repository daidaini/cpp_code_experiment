#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <iostream>

using namespace std;

int main()
{
    try
    {
        // 创建 MySQL 驱动实例
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        // 获取 MySQL 驱动
        driver = sql::mysql::get_mysql_driver_instance();

        // 连接数据库
        con = driver->connect("tcp://127.0.0.1:3306", "root", "123456"); // 替换为自己的数据库连接信息

        // 选择数据库
        con->setSchema("cloud_order"); // 使用 test 数据库

        // 创建 SQL 查询语句
        sql::Statement *stmt = con->createStatement();
        if (stmt == nullptr)
        {
            cout << "createStatement failed\n";
        }

        // 执行查询语句
        sql::ResultSet *res = stmt->executeQuery("SELECT STEP_KHH, STEP_JYMM, STEP_ZDXX FROM authed_accounts");
        if (res == nullptr)
        {
            cout << "executeQuery failed\n";
        }

        // 遍历查询结果
        while (res->next())
        {
            int id = res->getInt("STEP_KHH");
            string pwd = res->getString("STEP_JYMM");
            int zdxx = res->getInt("STEP_ZDXX");

            cout << "KHH: " << id << ", JYMM: " << pwd << ", ZDXX: " << zdxx << endl;
        }

        // 释放资源
        delete res;
        delete stmt;
        delete con;
        cout << "ends\n";
    }
    catch (sql::SQLException &e)
    {
        cout << "ERROR: " << e.what() << endl;
        return 1;
    }
    return 0;
}
