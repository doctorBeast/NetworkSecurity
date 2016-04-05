##第1章 了解SQL
###主键  
一列或者一组列其值能够唯一区分表中的每个行

作为主键满足的条件：  
- 任意两行都不具有相同的主键值(唯一性)  
- 每个行都必须具有同一个主键值(主键列不能为NULL)

主键的好习惯：  
- 不更新主键列中的值
- 不重用主键列的值
- 不在主键列中使用可能会更改的值

###实践
本书使用的样例表：  
下载[SQL脚本](http://www.forta.com/books/0672327120/mysql_scripts.zip)

创建数据库：

    mysql> create database daige;
    Query OK, 1 row affected (0.05 sec)

选中数据库：

    mysql> use daige;
    Database changed

导入cteate.sql文件：

    mysql> source create.sql
    Query OK, 0 rows affected (0.33 sec)
    Query OK, 0 rows affected (0.10 sec)
    .....
    Records: 0  Duplicates: 0  Warnings: 0

查看表：

    mysql> show tables;
    +-----------------+
    | Tables_in_daige |
    +-----------------+
    | customers       |
    | orderitems      |
    | orders          |
    | productnotes    |
    | products        |
    | vendors         |
    +-----------------+
    6 rows in set (0.02 sec)

查看表结构：

    mysql> desc customers;
    +--------------+-----------+------+-----+---------+----------------+
    | Field        | Type      | Null | Key | Default | Extra          |
    +--------------+-----------+------+-----+---------+----------------+
    | cust_id      | int(11)   | NO   | PRI | NULL    | auto_increment |
    | cust_name    | char(50)  | NO   |     | NULL    |                |
    | cust_address | char(50)  | YES  |     | NULL    |                |
    | cust_city    | char(50)  | YES  |     | NULL    |                |
    | cust_state   | char(5)   | YES  |     | NULL    |                |
    | cust_zip     | char(10)  | YES  |     | NULL    |                |
    | cust_country | char(50)  | YES  |     | NULL    |                |
    | cust_contact | char(50)  | YES  |     | NULL    |                |
    | cust_email   | char(255) | YES  |     | NULL    |                |
    +--------------+-----------+------+-----+---------+----------------+
    9 rows in set (0.04 sec)

导入populate.sql：

    mysql> source populate.sql
    Query OK, 1 row affected (0.02 sec)

查看是否导入成功：

    mysql> select * from customers;
    +---------+----------------+---------------------+-----------+------------+----------+--------------+---
    | cust_id | cust_name      | cust_address        | cust_city | cust_state | cust_zip | cust_country | cu
    +---------+----------------+---------------------+-----------+------------+----------+--------------+---
    |   10001 | Coyote Inc.    | 200 Maple Lane      | Detroit   | MI         | 44444    | USA          | Y
    |   10002 | Mouse House    | 333 Fromage Lane    | Columbus  | OH         | 43333    | USA          | Je
    |   10003 | Wascals        | 1 Sunny Place       | Muncie    | IN         | 42222    | USA          | Ji
    |   10004 | Yosemite Place | 829 Riverside Drive | Phoenix   | AZ         | 88888    | USA          | Y
    |   10005 | E Fudd         | 4545 53rd Street    | Chicago   | IL         | 54545    | USA          | E
    +---------+----------------+---------------------+-----------+------------+----------+--------------+---
    5 rows in set (0.00 sec)

不使用mysql命令行，使用window命令行导入：

    # mysql -u root -p daige < d:/populate.sql
    Enter password: ******

导入完成，这些表以后章节会使用

##第2章 MySQL简介
mysql工具：
- mysql命令行
- mysql administrator
- mysql query browser

##第3章 使用MySQL
连接： 需要 主机名+端口+合法用户名+[用户密码]  
选择数据库： `use daige;`  
了解数据库和表： 
- `show databses;` 列出数据库
- `show tables;`   列出表  
- `show columns from customers;` 返回表结构(功能同describe) desc实际调用此语句
- `show status;` 显示服务器状态
- `show create database daige;` 显示创建daige数据库的语句
- `show create table daige.customers;` 显示创建customers表的语句
- `show grants for root@localhost;` 显示root的权限
- `show errors` 显示错误信息
- `show warnings` 显示警告信息 
- `show engines;`  显示安装后的可用存储引擎和默认引擎
- `show table status;` 显示表信息
- `show variables;`  显示系统变量的名称和值

结果如：

    mysql> show create database daige;
    +----------+------------------------------------------------------------------+
    | Database | Create Database                                                  |
    +----------+------------------------------------------------------------------+
    | daige    | CREATE DATABASE `daige` /*!40100 DEFAULT CHARACTER SET latin1 */ |
    +----------+------------------------------------------------------------------+
    1 row in set (0.00 sec)

    mysql> show create table daige.customers;
    --------------------------------------------------------------------------+
    | Table     | Create Table
                                                                              
    --------------------------------------------------------------------------+
    | customers | CREATE TABLE `customers` (
      `cust_id` int(11) NOT NULL AUTO_INCREMENT,
      `cust_name` char(50) NOT NULL,
      `cust_address` char(50) DEFAULT NULL,
      `cust_city` char(50) DEFAULT NULL,
      `cust_state` char(5) DEFAULT NULL,
      `cust_zip` char(10) DEFAULT NULL,
      `cust_country` char(50) DEFAULT NULL,
      `cust_contact` char(50) DEFAULT NULL,
      `cust_email` char(255) DEFAULT NULL,
      PRIMARY KEY (`cust_id`)
    ) ENGINE=InnoDB AUTO_INCREMENT=10006 DEFAULT CHARSET=latin1 |
    +-----------+--------------------------------------------------------------
    --------------------------------------------------------------------------+

    mysql> show grants;
    +----------------------------------------------------------------------------------------------------------------------------------------+
    | Grants for root@localhost                                                                                                              |
    +----------------------------------------------------------------------------------------------------------------------------------------+
    | GRANT ALL PRIVILEGES ON *.* TO 'root'@'localhost' IDENTIFIED BY PASSWORD '*6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9' WITH GRANT OPTION |
    | GRANT PROXY ON ''@'' TO 'root'@'localhost' WITH GRANT OPTION                                                                           |
    +----------------------------------------------------------------------------------------------------------------------------------------+
    2 rows in set (0.04 sec)

##第4章 检索数据
###select
- `select prod_name from products;` 检索单个列
 
- `select prod_id,prod_name,prod_price from products;` 检索多个列

- `select * from products;` 检索所有列

- `select distinct vend_id from products;` 检索不同的行distinct表示返回不同的值
  distinct作用于所有列，并不仅仅是它后面的列 

- `select prod_name  from products limit 5,5;` 限制结果 输出从行5(从0开始计数)开始的5行和 `select prod_name  from products limit 5 offset 5;`同效果

-  `select products.prod_name from daige.products;` 完全限定表名和数据库名


##第5章 排序检索数据
###order by
- `select prod_name from products order by prod_name;` 单列排序

- `select prod_id,prod_price,prod_name from products order by prod_price, prod_name;` 多列排序 注意是在prod_price相同时，对prod_name排序

- `select prod_id,prod_price,prod_name from products order by prod_price desc,prod_name `; 多列prod_price降序排序  prod_name默认为升序  desc只对前面字段降序

- `select prod_price from products order by prod_price desc limit 1;` 输出单价最高的商品 limit应该在oder by后面

##第6章 过滤数据
###where
- `select prod_id,prod_name,prod_price from products where prod_price=2.5;` 输出prod_price为2.5的id，name和价格　 where 应该在order by之前

- `select prod_name,prod_price from products where prod_price <= 10;` 输出价格小于等于10的name,price

###where子句操作符
- =   等于
- <>  不等于
- !=   不等于
- <    小于
- <=   小于等于
- >     大于
- >=    大于等于
- between  在指定的两个值之间

- `select vend_id,prod_name from products where vend_id <> 1003;` 
  列出vend_id不为1003  　使用 != 一样的效果

- `select  prod_name,prod_price from products where prod_price between 5 and 10;` 列出prod_price在5到10(包括5和10)之间的name和price

- `select prod_name,prod_price from products where prod_price is NULL;` 
  列出price为NULL的name和price 　NULL表示无值


##第7章 数据过滤
###组合where
- `select prod_id,prod_name,prod_price from products where vend_id=1003 and prod_price <= 10;`
  vend_id等于1003并且price小于等于10的id,name,price  and 多条件同时满足

- `select prod_name,prod_price from products where vend_id = 1003 or vend_id=1002;`
  vend_id为1002或者1003的name和price  or 任一条件满足

- `select prod_name,prod_price from products where (vend_id=1002 or vend_id=1003) and prod_price <=10;`
  组合上面两个 or和and混合时，SQL先执行and 所以需要括号括起来 

- `select prod_name,prod_price from products where vend_id in(1002,1003) order by prod_name;` 
  vend_id在1002到1003这个闭区间中 in 指定条件的范围里  
  in 比起or有几个好处：语法更清楚直观，次序易于管理，执行效率高，更易动态建立where语句

- `select prod_name,prod_price from products where vend_id not in(1002,1003) order by prod_name;` 
  not 否定它之后的条件


##第8章 用通配符进行匹配
###like
通配符是用来匹配一部分的特殊字符  
SQL支持的几种通配符
- `select prod_id,prod_name from products where prod_name like 'jet%';`   
  % 号 表示任何字符出现任何次数 这里`'jet%'`表示prod_name以jet开头  
  尾空格：尾空格可能会干扰通配符的匹配，最好的解决方法就是在后面加`%`    比如：'%anvil%'   
  %不能匹配NULL  

- `select prod_id,prod_name from products where prod_name like '_ ton anvil';`  
  _ 号 匹配单个字符

使用通配符的技巧：
- 不要过度使用通配符，如果能有其他操作符能到达目标就使用其他操作符，因为效率不高。
- 在确定使用通配符时，除非绝对有必要，不要把它们用于搜索模式的最开始处，这种模式最慢。
- 注意通配符的位置

##第9章 用正则表达式进行搜索
###正则表达式
- `select prod_name from products where prod_name regexp '.000' order by prod_name;`  
regexp 后面跟着的是一个 正则表达式 `.000` 中`.`表示匹配任意一个字符
 这里，使用like不能到达目的，因为like不使用通配符就表示匹配整个值

- `select prod_name from products where prod_name regexp '1000|2000' order by prod_name;`  
 `|` or匹配 表示匹配任意之一  所以1000和2000的都返回

- `select prod_name from products where prod_name regexp '[123] Ton' order by prod_name;`  
 `[123]`表示匹配1或者2或者3 另一种形式的or匹配

- `select prod_name from products where prod_name regexp '[1-5] Ton' order by prod_name;`   
  `[1-5]`范围匹配，匹配1-5的闭区间 

- `select vend_name from vendors where vend_name regexp '\\.' order by vend_name;`  
  `\\.` 匹配特殊字符 特殊字符必须要有转义前导`\\`   
  空白元字符:\\f \\n \\r \\t \\v

- ` select prod_name from products where prod_name regexp '\\([0-9] sticks?\\)' order by prod_name;`  
  `\\([0-9] sticks?\\)` 多个匹配 [0-9]匹配数字 sticks?匹配stick和sticks  
  重复元字符：
  - *  0个或多个匹配
  - +  一个或多个匹配
  - ?  0个或者多个匹配
  - {n} 指定数目匹配
  - {n,} 不少于指定数目的匹配
  - {n,m} 匹配数目的范围  m不大于255
  
- `select prod_name from products where prod_name regexp '[[:digit:]]{4}' order by prod_name;`  
- `[[:digit:]]{4}` 匹配任意4个数字  
 字符类：
 - [:alnum:] 任意字母和数字
 - [:alpha:] 任意字符
 - [:blank:] 空格和制表
 - [:cntrl:] ASCII控制字符
 - [:digit:] 任意数字
 - [:graph:] 任意可打印字符 不包括空格
 - [:lower:] 任意小写字母
 - [:print:] 任意可打印字符
 - [:punct:] 不包括[:alnum:]和[:cntrl:]的任意字符
 - [:space:] 包括空格在内的任意空白字符
 - [:upper:] 任意大写字母
 - [:xdigit:] 任意十六进制数字
 
- ` select prod_name from products where prod_name regexp '^[0-9\\.]' order by prod_name;`    
  `^[0-9\\.]` 匹配以0-9和.开头  
  定位元字符：
  - ^ 文本开始 ^还有一种用法：`[^0-9]` 表示不在在0-9中 ^在集合中表示否定
  - $ 文本结尾
  - [[:<:]] 词的开始
  - [[:>:]] 词的结尾

##第10章 创建计算字段
###concat()
- `select concat(vend_name,'(',vend_country,')') from vendors order by vend_name;`  
  Mysql使用concat函数来拼接结果，这和很多数据库使用+或者||不同 

- `select concat(rtrim(vend_name),'(',rtrim(vend_country),')') from vendors order by vend_name;`  
  - rtrim函数 移除字符串末尾的空白
  - trim函数  移除字符串末尾或者开头的空白
  - ltrim函数 移除字符串开头的空白

###as

- ` select concat(rtrim(vend_name),'(',rtrim(vend_country),')') as vend_title from vendors order by vend_name;`  
  as vend_title as 后面的字符串为查询的字段或者值的别名 该字段是计算字段 并不真实存在

- `select prod_id,quantity,item_price, quantity*item_price as expanded_price from orderitems where order_num=20005;`    
  `quantity*item_price as expanded_price` expanded_price为一个计算字段


<<[返回目录](../README.md) 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　>>[第11-20章](./Chapter11-20.md)