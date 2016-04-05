##第21章 创建和操作表
####创建表
利用cretae table创建表，需要给出以下的信息：
- 新表的名字 
- 表列的名字和定义，用逗号分隔

customers表的创建SQL:
 <pre><code>
  create table if not exists customer 
  (
     cust_id  int  NOT NULL AUTO_INCREMENT,
     cust_name char(50) NOT NULL,
     cust_address char(50)  NULL,
     cust_city char(50)  NULL,
     cust_state char(5)  NULL,
     cust_zip char(10)  NULL,
     cust_country char(50)  NULL,
     cust_contact  char(50)  NULL,
     cust_email    char(255) NULL,
     PRIMARY KEY (cust_id)
  ) ENGINE = InnoDB;
  </code></pre>

  在创建新表时，指定的表名必须不存在，否则出错。如果想在此表不存在时建表，可以使用 `if not exists customers `

####NULL值
NULL代表没有值，允许NULL值的列就允许在插入的时候可以不给出该列的值，不允许NULL值的列不接受该列没有值的行，即插入或者更新时必须有值  

NULL值代表没有值，和空串不一样，空串是一个有效的值  

####再说主键
主键必须唯一，表中的行和另外的行，主键必须不一样，主键能够唯一的标识那一行  

单列主键：
`primary key (vend_id)`  

多列主键：
`primary key (order_num,order_item)`

主键不能使用NULL值

####AUTO_INCREMENT
`cust_id  int not null AUTO_INCREMENT`  
使用AUTO_INCREMENT告诉Mysql，本列每当增加一列时，自动加1  
这样，cust_id就是唯一，可以做主键  

每个表只运行一个AUTO_INCREMENT的列，而且它必须被索引

如果需要手动更新AUTO_INCREMENT列的值，需要指定一个没有被使用过的值 
到了此值之后，将在此值的基础上增加  

得到AUTO_INCREMENT列的最后一个值，可以通过last_insert_id()函数来获得

####指定默认值
如果在插入行的时候，没有给出值，mysql允许使用指定的默认值，这个默认值是在建表的时候通过default指定 

`quantity int  not null default 1`  

mysql不允许函数作为默认值   

####引擎类型

指定引擎类型  
`ENGINE = InnoDB`  

mysql有多种引擎，默认引擎一般是MyISAM，最常见的引擎有：
- InnoDB 是一个可靠事务处理引擎，不支持全文本搜索 
- MEMORY 功能上同MyISAM，但是数据存储在内存中，速度很快，适合临时表
- MyISAM是一个性能极高的引擎，支持全文本搜索，不支持事务处理

注意：  
外键不能跨引擎，引擎不同的表不能引用不同引擎的表的外键  

####更新表
更新表的定义，可以使用alter table语句，更改表结构需要以下信息：  
- ALTER TABLE 之后要给出更改的表名
- 所做更改的列表

  <pre><code>
   alter table vendors add vend_phone char(20);
   alter table drop  culumn vend_phone;
  </code></pre>

增加列和删除列

增加外键：
 <pre><code>
  ALTER TABLE orderitems ADD CONSTRAINT fk_orderitems_orders FOREIGN KEY (order_num) REFERENCES orders (order_num);
  ALTER TABLE orderitems ADD CONSTRAINT fk_orderitems_products FOREIGN KEY (prod_id) REFERENCES products (prod_id);
  ALTER TABLE orders ADD CONSTRAINT fk_orders_customers FOREIGN KEY (cust_id) REFERENCES customers (cust_id);
  ALTER TABLE products ADD CONSTRAINT fk_products_vendors FOREIGN KEY (vend_id) REFERENCES vendors (vend_id);
 </code></pre>

 复杂的表结构要更改，需要手动删除过程：
 - 用新的列布局创建一个新表
 - 使用insert select语句把旧表复制数据到新表
 - 检验包含所需数据的新表
 - 重命名旧表
 - 用旧表原来的名字重命名新表
 - 根据需要，重新创建触发器，存储过程，索引和外键

使用alter table时需要很小心，可以在改动之前进行完整的备份  

####删除表
`drop table customers`  

####重命名表
`rename table customers to customer`  

rename还可以给多个表重命名：
`rename table backup_customers to customers, backup_vendors to vendors`;

##第22章 使用视图
####视图
视图是虚拟的表  
它与包含数据的表不一样，视图只包含使用时动态检索数据的查询  
  <pre><code>
  select cust_name,cust_contact
  from customers,orders,orderitems 
  where customers.cust_id=orders.cust_id 
        and orderitems.order_num=orders.order_num 
        and prod_id='TNT2';
  </code></pre>

此查询通过对某种特定的产品查询，需要跨几个表，进行连接之后才能得到，视图可以把这样一个跨表查询包装成一个虚拟的表productcustomers，那么查询就简单了：

 <pre><code>
  select cust_name,cust_contact
  from productcustomers
  where prod_id ='TNT2';
 </code></pre>

为什么要使用视图：  
- 重用SQL语句
- 简化复杂的SQL操作
- 使用表的组成部分而不是整个表
- 保护数据
- 更改数据格式和表示

视图的规则和限制：
- 视图必须唯一命名
- 对于可以创建的视图数目没有限制
- 为了创建视图，必须具有足够的访问权限
- 视图可以嵌套
- order by可以用到视图中，但是如果select也用了order by，那么视图的order by会被覆盖
- 视图不能索引，不能关联触发器或者默认值
- 视图和表一起使用


视图操作：
- create view 语句 创建
- show create view viewname 查看创建的视图的语句
- drop view viewname删除视图
- 更新视图 可先drop后create，也可create or replace view

创建：
  <pre><code>
  create view productcustomers 
  as select cust_name,cust_contact,prod_id 
  from customers,orders,orderitems 
  where customers.cust_id = orders.cust_id 
  and orderitems.order_num = orders.order_num;
  </code></pre>

查看语句：
`show create view productcustomers;`

视图结构：

    mysql> desc productcustomers;
    +--------------+----------+------+-----+---------+-------+
    | Field        | Type     | Null | Key | Default | Extra |
    +--------------+----------+------+-----+---------+-------+
    | cust_name    | char(50) | NO   |     | NULL    |       |
    | cust_contact | char(50) | YES  |     | NULL    |       |
    | prod_id      | char(10) | NO   |     | NULL    |       |
    +--------------+----------+------+-----+---------+-------+
    3 rows in set (0.11 sec)

使用：

    mysql> select cust_name,cust_contact from productcustomers where prod_id = 'TNT2';
    +----------------+--------------+
    | cust_name      | cust_contact |
    +----------------+--------------+
    | Coyote Inc.    | Y Lee        |
    | Yosemite Place | Y Sam        |
    +----------------+--------------+
    2 rows in set (0.00 sec)


使用视图重新格式化检索出的数据：
<pre><code>
 create view vendorlocations 
 as select concat(rtrim(vend_name),'(',rtrim(vend_country),')') 
   as vend_title 
   from vendors
   order by vend_name;
</code></pre>

使用：

    mysql> select * from vendorlocations;
    +------------------------+
    | vend_title             |
    +------------------------+
    | ACME(USA)              |
    | Anvils R Us(USA)       |
    | Furball Inc.(USA)      |
    | Jet Set(England)       |
    | Jouets Et Ours(France) |
    | LT Supplies(USA)       |
    +------------------------+
    6 rows in set (0.02 sec)


使用视图过滤不想要的数据：
<pre><code>
 create view customeremaillist 
 as select cust_id,cust_name,cust_email 
 from customers 
 where cust_email is not null;
</code></pre>

把email存在的cust_id,name,email组成一个视图，排除没有email的

使用：

    mysql> select * from customeremaillist;
    +---------+----------------+---------------------+
    | cust_id | cust_name      | cust_email          |
    +---------+----------------+---------------------+
    |   10001 | Coyote Inc.    | ylee@coyote.com     |
    |   10003 | Wascals        | rabbit@wascally.com |
    |   10004 | Yosemite Place | sam@yosemite.com    |
    |   10005 | The Fudds      | elmer@fudd.com      |
    +---------+----------------+---------------------+
    4 rows in set (0.08 sec)


使用视图与计算字段：
 <pre><code>
 create view orderitemsexpanded 
 as select order_num,prod_id,quantity,item_price,quantity*item_price 
     as expanded_price 
     from orderitems;
 </code></pre>

 使用：  

     mysql> select * from orderitemsexpanded where order_num = 20005;
    +-----------+---------+----------+------------+----------------+
    | order_num | prod_id | quantity | item_price | expanded_price |
    +-----------+---------+----------+------------+----------------+
    |     20005 | ANV01   |       10 |       5.99 |          59.90 |
    |     20005 | ANV02   |        3 |       9.99 |          29.97 |
    |     20005 | TNT2    |        5 |      10.00 |          50.00 |
    |     20005 | FB      |        1 |      10.00 |          10.00 |
    +-----------+---------+----------+------------+----------------+
    4 rows in set (0.02 sec)

####更新视图
视图通常是可更新的，你对视图增加或者删除行，实际上是对基表的增加或者删除  

并非所有视图都可以更新，如果mysql不能正确的被更新的基数据，那不允许更新  

不能进行视图的更新：
- 分组
- 联结
- 子查询
- 并
- 聚集函数
- distinct
- 导出列


##第23章 使用存储过程
####存储过程
存储过程简单来说就是为以后的使用而保存的一条或者多条mysql语句的集合  

为什么要使用存储过程：
- 通过把处理封装为容易使用的单元中，简化复杂的操作
- 由于不要求反复建立一系列处理步骤，保证了数据的完整性
- 简化对变动的管理，减少数据讹误
- 提高性能 存储过程比单独sql语句要快
- 存在一些只能用在单个请求中的mysql元素和特性，存储过程可以使用它们来编写功能更更灵活的代码  

简单来说，三个主要的好处，简单，安全和高性能  

存储过程的缺陷： 
- 对基本sql语句复杂，编写存储过程需要更高的技能，更丰富的经验
- 你可能没有创建存储过程的安全访问权限  

使用存储过程：

调用：
`call productpricing(@pricelow,@pricehight,@priceverage)`  

创建：  
 <pre><code>
   create procedure productpricing()
   begin
        select avg(prod_price) as priceaverage
        from products;
   end;
 </code></pre>

在命令行中输入`;`会被认为是结束，在存储过程输入中，我们需要使用`delimiter //`用`//`来代替`;`分号的作用  

    mysql> delimiter //
    mysql> create procedure productpricing()
        ->    begin
        ->         select avg(prod_price) as priceaverage
        ->         from products;
        ->    end
        -> //

查看存储过程：

    mysql> show procedure status;
    +-------+----------------+-----------+----------------+--------------------
    | Db    | Name           | Type      | Definer        | Modified
    +-------+----------------+-----------+----------------+--------------------
    | daige | productpricing | PROCEDURE | root@localhost | 2014-10-14 11:05:56
    +-------+----------------+-----------+----------------+--------------------

调用存储过程：

    mysql> call productpricing();
    +--------------+
    | priceaverage |
    +--------------+
    |    16.133571 |
    +--------------+
    1 row in set (0.77 sec)

删除存储过程：

`drop procedure productpricing` 

先判断是否存在：

`drop procedure if exists productpricing`

使用参数：  
变量：内存中一个特定的位置，用来临时存储数据  

 <pre><code>
  create procedure productpricing
  (
     out p1 decimal(8,2),
     out ph decimal(8,2),
     out pa decimal(8,2)
  )
  begin
      select min(prod_price)
      into pl
      from products;
      select max(prod_price)
      into ph
      from products;
      select avg(prod_price)
      into pa
      from products;
  end;

 </code></pre>

此存储过程有三个参数，pl,ph,pa  
MYSQL支持in，out，inout类型的参数  

调用：

    mysql> call productpricing(@pricelow,@pricehigh,@priceaverage)

输出变量：

    mysql> select @priceaverage
    -> //
    +---------------+
    | @priceaverage |
    +---------------+
    |         16.13 |
    +---------------+
    1 row in set (0.00 sec)


使用in和out参数：
<pre><code>
  create procedure ordertotal
  (
    in  onumber INT,
    out ototal DECIMAL(8,2)
  )
  begin
     select sum(item_price*quantity)
     from orderitems
     where order_num = onumber
     into ototal;
  end
</code></pre>

调用：

    mysql> call ordertotal(20005,@total)

显示：

    mysql> select @total//
    +--------+
    | @total |
    +--------+
    | 149.87 |
    +--------+
    1 row in set (0.00 sec)

更加智能的存储过程：
<pre><code>
 -- name: ordertotal
 -- Parameters: onumber = order number
 --             taxable = 0 if not taxable,1 if taxable 
 --             ototal = order total variable

 create procedure ordertotal 
 (
     in  onumber int,
     in  taxable boolean,
     out  ototal decimal(8,2)
 )comment 'obatain order total,optionally adding tax'
 begin
    -- declare variable for total
    declare total decimal(8,2);
    -- declar tax percentage
    declare taxrate int default 6;

    -- get the order total
    select sum(item_price *quantity)
    from orderitems
    where order_num = onumber
    into total;

    -- is this taxable
    if taxable then
        select total + (total/100*taxrate) into total;
    end if;

    -- add finally save to out variable
    select total into ototal;
    end;

</code></pre>

此存储过程增加了：
- `--`注释
- `boolean` 布尔值
- 默认值 default 6
- if 条件检查
- comment 备注

调用：

     mysql> call ordertotal(20005,0,@toral);
    Query OK, 1 row affected (0.00 sec)

显示：

    mysql> select @total;
    +--------+
    | @total |
    +--------+
    | 149.87 |
    +--------+
    1 row in set (0.00 sec)

检查存储过程：

`mysql> show create procedure ordertotal;`
会显示创建的语句  

##第24章 使用游标
####游标
我们知道mysql检索操作返回的是一组称为结果集的行，这样的一组返回的行都是和sql语句想匹配的行。使用简单的sql语句，是没有办法得到第一行，下一行或者前十行。有时候，需要在检索出来的行中前进或者后退一行或者多行，在这个时候，就需要使用游标  

游标是存储在mysql服务器上的数据库查询，不是一条select语句，而是被该语句检索出来的结果集  

游标主要用于交互式应用，用户需要滚动屏幕上的数据，并对数据进行浏览或做出更改  

注意：mysql中的游标只能用于存储过程

使用游标：
- 在使用游标，必须声明它。这个过程实际上没有检索数据，只是定义要使用的select语句
- 一旦声明之后，必须打开游标以供使用。这个过程，select语句把数据实际检索出来
- 对于填写数据的游标，根据需要取出各行
- 结束使用游标时，必须关闭游标

创建游标：
游标使用declare语句创建

    mysql> create procedure processorders()
        -> begin
        ->    declare ordernumbers cursor
        ->    for
        ->    select order_num from orders;
        ->  end;
        -> //
    Query OK, 0 rows affected (0.58 sec)

此语句定义了一个名为ordernumbers的游标  


打开和关闭游标：  
`open ordernumbers;`  打开  

`close ordernumbers;`  关闭

 <pre><code>
  create procedure processorders()
  begin
     declare ordernumbers cursor
     for
     select order_num from orders;
     open ordernumbers;
     close ordernumbers;
  end;
  </code></pre>

使用游标数据：  
在一个游标被打开后，可以使用fetch语句分别访问它的每一行，fetch指定检索什么数据，检索出来的数据存储在什么地方，它还向前移动游标中的内部行指针，使下一条fetch语句检索下一行  

  <pre><code>
  create procedure processorders()
  begin
     declare o int;
     declare ordernumbers cursor
     for
     select order_num from orders;
     open ordernumbers;
     fetch ordernumbers into o;
     close ordernumbers;
  end;
  </code></pre>
使用fetch用来检索当前行的order_num，并存到一个名为o的局部变量中  
<pre><code>
  create procedure processorders()
  begin
     declare done boolean default 0;
     declare o int;
     declare ordernumbers cursor
     for
     select order_num from orders;
     declare continue handler for sqlstate '02000' set done = 1;
     open ordernumbers;
     repeat
        fetch ordernumbers into o;
      until done end repeat;
     close ordernumbers;
  end;
  </code></pre>
这个例子，循环检索数据，从第一行到最后一行  

 <pre><code>
  mysql> create procedure processorders()
    ->   begin
    ->      declare done boolean default 0;
    ->      declare o int;
    ->      declare t decimal(8,2);
    ->
    ->      declare ordernumbers cursor
    ->      for
    ->      select order_num from orders;
    ->
    ->      declare continue handler for sqlstate '02000' set done = 1;
    ->
    ->      create table if not exists ordertotals
    ->      (order_num int,total decimal(8,2));
    ->
    ->      open ordernumbers;
    ->      repeat
    ->         fetch ordernumbers into o;
    ->         call ordertotal(o,1,t);
    ->         insert into ordertotals(order_num,total) values(o,t);
    ->
    ->       until done end repeat;
    ->      close ordernumbers;
    ->   end; //
Query OK, 0 rows affected (0.01 sec)
   </code></pre>
上面的例子包含了存储过程，游标，逐行处理以及存储过程调用其他存储过程  

##第25章 使用触发器
####触发器
如果你想要某条语句在事件发生时自动执行，那你需要触发器  

触发器是mysql响应以下任意语句而自动执行的一条mysql语句（或者一组语句）  
- delete
- insert
- update

创建触发器需要的信息：
- 唯一的触发器名
- 触发器关联的表
- 触发器应该响应的活动(delete insert 或者 update)
- 触发器何时执行

触发器保持每个数据库唯一  

创建触发器：

    CREATE TRIGGER 触发器名 BEFORE|AFTER 触发事件
    ON 表名 FOR EACH ROW 执行语句

例子：
`create trigger newproduct after insert on products for each row select 'product added';`  

注意:在mysql5中此语句不能通过，我们在products表中增加time列来方便演示  
` alter table products add time datetime`  

修改之后的触发器语句：

`create trigger newproduct after insert on products for each row  insert into time values(now());`  

查看触发器：

    mysql> show triggers\G;
    *************************** 1. row ***************************
                 Trigger: newproduct
                   Event: INSERT
                   Table: products
               Statement: insert into time  values(now())
                  Timing: AFTER
                 Created: NULL
                sql_mode: NO_ENGINE_SUBSTITUTION
                 Definer: root@localhost
    character_set_client: gbk
    collation_connection: gbk_chinese_ci
      Database Collation: latin1_swedish_ci
    1 row in set (0.01 sec)

触发器可以选择在一个操作发生之前或者之后执行  
只有表才支持触发器，视图和临时表都不支持  

如果触发器失败，mysql将不执行请求的操作，如果before触发器或者语句本身失败，mysql将不执行after触发器 

删除触发器：  
`drop tigger newproduct;`  

触发器不能更新或者覆盖，要修改只能删除后重新创建  

使用触发器：  

insert触发器  
- 在insert触发器代码内，可引用一个名为new的虚拟表，访问被插入的行  
- 在before insert触发器中，new中的值额可以被更新
- 对于auto_increment列，new在insert执行之前包含0，在insert执行之后包含新的自动生成值

我们先对orders表增加一个可以为NULL的year列，然后创建一下触发器：

  <pre><code>
   create trigger neworder after insert on orders 
   for each row 
   insert into year values(year(now()));
  </code></pre>


创建有多个执行语句的触发：

    CREATE TRIGGER 触发器名 BEFORE|AFTER 触发事件
    ON 表名 FOR EACH ROW
    BEGIN
        执行语句列表
    END

例子：

 <pre><code>
   create trigger neworder after insert on orders 
   for each row 
   begin
   insert into year values(year(now()));
   insert into month values(month(now()));
   end;
  </code></pre>

输出：

    mysql> delimiter //
    mysql> create trigger neworder after insert on orders
        ->    for each row
        ->    begin
        ->    insert into year values(year(now()));
        ->    insert into month values(month(now()));
        ->    end;
        -> //
    Query OK, 0 rows affected (0.01 sec)
    mysql> delimiter ;


从tiggers表中查看触发器信息：

    mysql> SELECT * FROM information_schema.triggers\G
    *************************** 1. row ***************************
               TRIGGER_CATALOG: def
                TRIGGER_SCHEMA: daige
                  TRIGGER_NAME: neworder
            EVENT_MANIPULATION: INSERT
          EVENT_OBJECT_CATALOG: def
           EVENT_OBJECT_SCHEMA: daige
            EVENT_OBJECT_TABLE: orders
                  ACTION_ORDER: 0
              ACTION_CONDITION: NULL
              ACTION_STATEMENT: begin
       insert into year values(year(now()));
       insert into month values(month(now()));
       end
            ACTION_ORIENTATION: ROW
                 ACTION_TIMING: AFTER
    ACTION_REFERENCE_OLD_TABLE: NULL
    ACTION_REFERENCE_NEW_TABLE: NULL
      ACTION_REFERENCE_OLD_ROW: OLD
      ACTION_REFERENCE_NEW_ROW: NEW
                       CREATED: NULL
                      SQL_MODE: NO_ENGINE_SUBSTITUTION
                       DEFINER: root@localhost
          CHARACTER_SET_CLIENT: gbk
          COLLATION_CONNECTION: gbk_chinese_ci
            DATABASE_COLLATION: latin1_swedish_ci

注意，不需要某个触发器的时候，必须删除，不然会出现混乱  

delete触发器：
- 在delete触发器代码中，你可以引用一个名为OLD的虚拟表，访问被删除的行  
- OLD中的值全都是只读，不能更新
<pre><code>
create trigger deleteorder before delete on orders
for each row
begin
    insert into archiver_orders(order_num,order_date,cust_id)
    values(OLD.order_num,OLD.order_date,OLD.cust_id);
end;
</code></pre>

此delete触发器的好处是，如果订单不能存档，delete本身会被放弃  

update触发器：
- 在update触发器代码中，你可以引用一个名为OLD的虚拟表来访问以前的值，引用一个名为NEW的虚拟表来访问更新后的值  
- 在before update触发器，NEW中的值也可能被更新
- OLD的值全是只读，不能更新

  <pre><code>
  create trigger updatevendor before update on vendors
  for each row
  set NEW.vend_state = Upper(NEW.vend_state);
  </code></pre>

  使用触发器的事项：
  - mysql5的触发器相当的初级
  - 创建触发器需要特殊的安全访问权限，触发器的执行是自动的  
  - 应该用触发器来保证的数据的一致性(大小写，格式等) 
  - 触发器的一种非常有意义的使用是创建审计跟踪



##第26章 管理事务处理
事务处理可以用来维护数据库的完整性，它保证成批的mysql操作要么安全执行，要么完全不执行  
注意：并不是所有的数据引擎都支持事务，最常用的MyISAM和InnoDB中，MyISAM不支持事务  
利用事务处理，可以保证一组操作不会中途停止，它们或者作为整体执行，或者完全不执行。如果没有错误，整组语句提交给数据库表，如果发生错误，那会进行回退以恢复数据库到一个安全的状态  

事务处理的几个词汇：
- 事务 指一组sql语句
- 回退 指撤销指定sql语句的过程
- 提交 指将为存储的sql语句结果写入到数据库表
- 保留点 指事务处理中设置的临时占位符，你可以对它发布回退

####rollback
mysql的rollback命令用来回退mysql语句
 <pre><code>
  select * from ordertotals;
  start transaction; //事务开始
  delete from ordertotals;
  select * from ordertotals;
  rollback;  //回退
  select * from ordertotals;
 </code></pre>
事务处理用来管理insert,update和delete语句，但是不能回退create和drop

####commit
一般的mysql语句都是直接针对数据库表执行和编写，这就是所谓的隐含提交，即提交操作是自动进行的

在事务处理中，提交不会隐含的进行，需要进行明确的提交，使用commit语句  

 <pre><code>
 start transaction;
 delete from orderitems where order_num = 20010;
 delete from orders where order_num = 20010;
 </code></pre> 

如果第一条delete起作用，第二条失败，则delete不会提交  

####使用保留点
简单的rollback和commit语句就可以写入或者撤销整个事务处理，复杂的事务处理可能需要部分提交或回退  

创建保留点：
`savepoint delete1` 

每个保留点都取一个唯一名字，方便回退，如果要回退到保留点，使用：
`rollback to delete1`

释放保留点：
保留点在事务处理完成之后自动释放，或者也可以用:
`release savepoint`  

如果需要mysql不自动提交，可以使用以下的语句：

`set autocommit = 0`  

这样，知道autocommit为真才会自动提交  

##第27章 全球化和本地化
数据库表被用来存储和检索数据。不同的语言和字符集需要以不同的方式存储和检索，
mysql需要适应不同的字符集，来适应不同的排序和检索数据的方法  

概念：
- 字符集  为字母和符号的集合
- 编码为某个字符集成员的内部表示
- 校对 为规定字符如何比较的指令

-  `show character set`  显示所有可用的字符集
-  `show collation` 显示所有可用的校对 
-  `show variables like 'character%'` 
-  `show variables like 'collation%'` 

创建表给定字符集和校对：
  <pre><code>
   create table mytable
   (
      test1 INT,
      test2 varchar(10)
   ) default character set hebrew
     collate hebrew_general_ci;
  </code></pre>

此语句创建一个包含两列的表，并且指定一个字符集和一个校对顺序  

对特定的列指定字符集和校对：
<pre><code>
  create table mytable
  (
    test1 INT,
    test2  varchar(10),
    test3  varchar(10) character set latin1 collate latin1_general_ci
  )default character set hebrew
  collate hebrew_general_ci;
</code></pre>


##第28章 安全管理
####访问控制
mysql服务器的安全基础：  
用户应该对他们需要的数据具有适当的访问权，既不能多也不能少  
- 多数用户只需要对表进行读和写，但少数用户甚至需要能创建和删除表
- 某些用户需要读表，但可能不需要更新表
- 你可能想允许用户添加数据，但不允许他们删除数据
- 某些用户可能需要处理用户账户的权限，但多数用户不需要
- 你可能想让用户通过存储过程访问数据，但不允许他们直接访问数据
- 你可能需要根据用户登录的地点来限制对某些功能的访问

防止无意的错误：  
访问控制的目的不仅仅是防止用户的恶意企图，还用来保证用户不能执行他们不应该执行的语句  

不能使用root来登录，只有在绝对需要时才使用 

####管理用户
mysql用户账户和信息存储在名为mysql的mysql数据库中  
需要直接访问的时机就是获取所有用户账户列表  

    mysql> use mysql;
    Database changed
    mysql> select user from user;
    +------+
    | user |
    +------+
    | root |
    | root |
    |      |
    | pma  |
    | root |
    +------+
    5 rows in set (0.22 sec)


创建用户：

    mysql> create user hello identified by 'xxxxx;

identified by ‘password’ 表示用户口令，mysql在保存之前会先加密  

使用grant或者insert也可以创建用户账户，但是不建议这样做  

重命名：

    mysql> rename user hello to helloworld;

删除：

    mysql> drop user helloworld;

设置访问权限：
在创建账户之后，需要分配访问权限 

显示账户的权限：

    mysql> show grants for hello;  
    +----------------------------------
    | Grants for hello@%
    +----------------------------------
    | GRANT USAGE ON *.* TO 'hello'@'%'
    +----------------------------------
    1 row in set (0.00 sec)

`USAGE ON *.*` 表示没有任何权限  

使用grants分配权限：
- 要授予的权限
- 被授予访问权限的数据库或者表
- 用户  

授予hello daige数据库的所有表的查询权限 

    mysql> grant select on  daige.* to hello;

查看权限：

    mysql> show grants for hello;  
    +-----------------------------------
    | Grants for hello@%
    +-----------------------------------
    | GRANT USAGE ON *.* TO 'hello'@'%' IDENTIFIED BY PASSWORD
    | GRANT SELECT ON `customers`.* TO 'hello'@'%'
    | GRANT SELECT ON `daige`.* TO 'hello'@'%'

grant的反操作是revoke，用它来撤销特定的权限

    mysql> revoke select on customers.* from hello;


    mysql> show grants for hello;
    +---------------------------------------------------------
    | Grants for hello@%
    +---------------------------------------------------------
    | GRANT USAGE ON *.* TO 'hello'@'%' IDENTIFIED BY PASSWORD
    | GRANT SELECT ON `daige`.* TO 'hello'@'%'
    +---------------------------------------------------------
权限被撤销  

grant和revoke可以在几个层次控制访问权限：
- 整个服务器 使用grant all 和 revoke all
- 整个数据库 使用 on database.*
- 特定的表   使用on database.table
- 特定的列
- 特定的存储过程

权限分类：
- all
- alter
- alter routine
- create
- create routine
- create temporary tables
- create user
- create view
- delete
- drop
- execute
- file
- grant option
- index
- insert
- lock tables
- process
- reload
- replication client
- replication slave
- select
- show databases
- show view
- shutdown
- super
- update
- usage

grant可以一次授权多个权限，用逗号隔开  

更改口令：

    mysql> set password for hello = password('hello');

口令必须传到password函数进行加密  


##第29章 数据库维护
####备份数据
mysql的数据需要经常备份，由于mysql数据库是基于磁盘文件，普通的备份系统和例程就能备份mysql的数据  

- 使用命令行使用程序mysqldump
- 使用mysqlhotcopy把一个数据库复制所有数据
- 使用mysql的backup table 或者 select into outfile转存所有数据到某个外部文件

为了保证所有数据被写到磁盘，可能需要在进行备份之前使用flush tables 语句  

####进行数据库维护
mysql提供了一系列的语句，可以用来保证数据库正确和正常运行  

- `analyze table` 用来检查表键是否正确
- `check table` 用来针对许多问题对表进行检查
- `repair table`  如果myiasm表访问产生不正确和不一致的结果，可能需要用repair table来修复相应的表
- `optimize table` 如果从一个表中删除大量的数据 应该使用optimize table来收回所用的空间，从而优化表的性能

表修复：
mysql进程在一个写入中被杀死，计算机意外关闭，硬件错误都会造成mysql表的损坏，损坏之后需要对表就行修复  

使用mysqlcheck:

    #mysqlcheck -uuser -ppassword database  table  -c  #检查单个表是否损坏
    #mysqlcheck -uuser -ppassword database  -c  #检查整个库那些表损坏

首先检查数据库的那些表损坏，如果能定位到那张表损坏可以直接对表修复

    #mysqlcheck -uuser -ppassword database  table  -r # 修复数据表
    #mysqlcheck -uuser -ppassword database   -r # 修复整个数据库

使用myisamchk：
Myisamchk是MyISAM表维护的一个非常实用的工具。可以使用myisamchk实用程序来获得有关数据库表的信息或检查、修复、优化他们。myisamchk适用MyISAM表(对应.MYI和.MYD文件的表)

检查表：

    myisamchk -e wpusers.MYI

修复表：

    myisamchk -r wpusers.MYI

注意使用myisamchk需要停止mysql服务并备份数据库文件  

####诊断启动问题
mysqld命令行选项：
- --help 显示帮助
- --safe-mode 装载减去某些最佳配置的服务器
- --verbose 显示全本文消息
- --version 显示版本

####查看日志文件
mysql日志文件有几种：
- 错误日志 hostname.errr 位于data目录
- 查询日志  hostname.log 位于data目录
- 二进制日记 hostname.bin 位于data目录
- 缓慢查询日志 hostname-slow.log 位于data目录

使用日志时，可用flush logs语句来刷新和重新开始所有日志文件

##第30章 改善性能
- mysql具有特定的硬件建议，在生产环境，应该坚持遵循这些硬件建议 
- mysql初始化的配置，在开始的时候通常很好，但过段时间后你可能需要调整内存分配，缓冲区大小等 
- mysql经常同时执行多个任务，如果某些任务执行缓慢，那所有请求都会执行缓慢，可以通过show processlist显示所有活动进程，还可以kill掉某个特定进程  
- 总是有不止一种方法编写同一条select语句，应该实验联结，并，子查询，找到最佳的方法
- 使用explain语句让mysql解释它将如何执行一条select语句
- 一般来说，存储过程执行比一条一条执行更快
- 应该总是使用正确的数据类型
- 绝不要检索比需求还要多的数据 不要使用select *
- 有的操作支持一个可选的delayed 如果使用它，将把控制立即返回给调用程序，并一单有可能就实际执行该操作
- 在导入数据时，应该关闭自动提交，你可能会删除索引，然后在导入完成后再重建它们
- 必须索引数据库表以改善数据检索的性能
- 你的select还使用复杂的or条件么？通过使用多条select语句和连接它们的union语句，性能可以极大的改进
- 索引改善数据检索的性能，但损害数据的插入，删除和更新的性能
- like很慢，一般来说最好使用fulltext而不是like
- 数据库是不断变化的视图，一组优化良好的表一会可能就面目全非，表的使用和内容的更改，理想的优化和配置也会改变
- 最重要的规则，每天规则在某些条件下都会被打破 

<<[第11-20章](./Chapter11-20.md) 