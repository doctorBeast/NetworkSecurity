##第11章 使用数据处理函数
##函数  
函数没有SQL的可移植性强  
大多数SQL实现支持一下类型的函数：
- 处理文本串
- 数值之间算数操作
- 处理日期和时间
- 返回DBMS的信息

####文本处理函数
- left 返回串左边的字符
- length 串的长度
- locate  找出串的一个子串
- lower  转换为小写
- ltrim  去掉左边空格
- right  返回串右边的字符
- rtrim  去掉右边空格
- soundex 将文本转换为语音表示的字母数字模式算法 比较发音
- substring  返回子串字符
- upper   转换为大写

- `select vend_name,upper(vend_name) as vend_name_upcase from vendors  order by vend_name;`   

- `select cust_name,cust_contact from customers where soundex(cust_contact) =soundex('Y Lie');`  

    <pre><code>
     mysql> select cust_name,cust_contact from customers 
            where soundex(cust_contact) =soundex('Y Lie');
    +-------------+--------------+
    | cust_name   | cust_contact |
     +-------------+--------------+
     | Coyote Inc. | Y Lee        |
     +-------------+--------------+
     1 row in set (0.00 sec)
    </pre></code>

   比较Y Lie发音，找到 Y Lee  

####日期和时间处理函数
- addDate  增加一个日期
- addtime  增加一个时间
- curdate  当前日期
- curtime  当前时间
- date     返回日期时间的日期部分
- datediff  计算两个日期之差
- date_add  日期运算
- date_formate  格式化日期
- day        返回一个日期的天数
- dayofweek  返回一个日期对应的星期几
- hour       返回一个时间的小时部分
- minute     返回一个时间的分钟部分
- month      返回一个日期的月份
- now        返回当前日期和时间
- second     返回一个时间的秒部分
- time       返回一个日期时间的时间部分
- year       返回一个日期的年份部分

在mysql中，日期的格式必须是yyyy-mm-dd

- `select cust_id,order_num from orders where order_date='2005-09-01';`  
   查询order_date为2005-09-01的id和num order_date为datetime类型

- `select cust_id,order_num from orders where date(order_date)='2005-09-01';`  
  可能`order_date='2005-09-01'`并不可靠，所以date(order_date)提取日期来比较 

- `select cust_id,order_num from orders where date(order_date) between '2005-09-01' and '2005-09-30';`  
  匹配月份的具体天数  

- `select cust_id,order_num from orders where year(order_date)=2005 and month(order_date)=9;`  
  从order_date中提取年月来匹配 

####数值处理函数
- abs
- cos
- mod
- pi
- rand
- sin
- sqrt
- tan


##第12章 数据汇总

####聚集函数
- avg    返回某列的平均数
- count  返回某列的行数
- max    返回某列的最大值
- min    返回某列的最小值
- sum    返回某列值之和


- `select avg(prod_price) as avg_price from products;`  
   avg会忽略值为NULL的行

- `select  count(*) as num_cust from customers;`  
   count(*)对表中行的数目进行计数，包括值为NULL的行

- `select count(cust_email) as num_cust from customers;`  
   count(cust_email)对cust_email进行计数，会忽略是NULL值的行

- `select max(prod_price) from products;`  
  max可以对非数值数据使用，返回的是列排序的最后一行 会忽略NULL的行  

- `select min(prod_price) as min_price from products;`  
  min对非数据使用，返回列排序的最前面的行  会忽略NULL的行 

- `select sum(quantity) as items_ordered from orderitems where order_num = 20005;`  
  sum 返回的是所有物品的数量之和，如果要返回总计金额需要sum(quantity*item_price)   sum忽略NULL值的行

####聚集不同值
- all 对所有执行计算  默认
- distinct 只包含不同的值  不能在count(*)使用

- `select avg(distinct prod_price) as avg_price from products where vend_id = 1003;`  
   这里使用distinct 来表示不同的产品价格  


####组合聚集函数
 <pre><code>
select count(*) as num_items ,min(prod_price) as price_min,
                              max(prod_price) as price_max,
                              avg(prod_price) as price_avg 
from products;
</pre></code>

##第13章 分组数据

####数据分组
- `select vend_id ,count(*) as num_prods from products group by vend_id;`  
   使用group by vend_id对vend_id进行分组统计  

   group by的注意事项：
   - group by子句可以包含任意数目的列  
   - 如果在group by子句中有嵌套分组，数据将在最后规定分组上汇总
   - group by子句中列出的每个列都必须是检索列或者是有效表达式
   - 除了聚集语句，select中的每个列都必须在group by子句中给出
   - 如果分组中有NULL值，NULL作为一个分组返回，如果列有多行NULL值，会被分为一组
   - group by 子句必须在where 之后，order by 之前
   
- `select vend_id,count(*) as num_prods from products group by vend_id  with rollup;`  
   使用with rollup 可以得到每个分组和分组汇总的级

####过滤分组
- `select cust_id,count(*) as orders from orders group by cust_id having count(*) >= 2;`  
  where是过滤行，having是过滤分组 having cout() >= 2表示过滤2个订单以上  
  where 是在分组前就过滤，having 在数据分组之后过滤  

-  `select vend_id,count(*) as num_prods  from products where prod_price >= 10  group by vend_id having count(*) >= 2;`  
   where先过滤行，prod_price >= 10   
   having 后过滤vend_id分组  count(*) >= 10 

####分组和排序
group by 和 order by的差别：  
- order by是排序输出  group by 是分组行
- order by 任意列都可以使用 group by 只能选择列或者表达式列  
- order by 不一定和聚集函数一起使用 group by 在聚集函数使用时必须使用


-  `select order_num,sum(quantity * item_price) as ordertotal from orderitems group by order_num having sum(quantity * item_price) >= 50 order by ordertotal;`    
   group by按订单号分组，order by排序订单总计  


####select子句的顺序
- select  要返回的列或者表达式
- from    检索数据的表
- where   行过滤 
- group by  分组
- having    分组过滤
- order by  排序
- limit     检索行数


##第14章 使用子查询
####子查询
- `select cust_id from orders where order_num in (select order_num from orderitems where prod_id='TNT2');`    
  这里把子查询查询出来的order_num作为主查询的范围  
  子查询都是由内向外处理   
  格式化SQL:就是把复杂的SQL适当的分解成多行和适当缩进  
  <pre><code>
   select cust_name,cust_contact 
   from customers 
   where cust_id in ( 
                      select cust_id 
                      from orders 
                      where order_num in ( 
                                            select order_num 
                                            from orderitems 
                                            where prod_id = 'TNT2'
                                          )
                     );

   </code></pre>
  注意：子查询中，列必须匹配  

- 作为计算字段使用子查询  
  <pre><code>
  select cust_name,cust_state,(
                                  select count(*) 
                                  from orders 
                                  where orders.cust_id = customers.cust_id 
                                ) as orders 
  from customers
  order by cust_name;
  </code></pre>
  这里的`(select count(*) from orders  where orders.cust_id = customers.cust_id  ) as orders` 查询出来当作select的一个查询字段显示出来  
  跨表查询需要完全限定列名  


##第15章 联结表

####外键
外键为某个表中的一列，它包含另一表中的主键值，定义两个表之间的关系  

####联结
分解数据为多个表能够有效的存储，更方便的处理，也有更好的伸缩性  
数据存储在多个表中，单条select查询需要使用联结  
联结是一种机制，用来在一条select语句中关联表  

- `select vend_name,prod_name,prod_price from vendors,products where vendors.vend_id = products.vend_id order by vend_name,prod_name;`  
 注意这里的vend_name和prod_name,prod_price是在不同的表中，
 如果要查询出来，需要指定`where vendors.vend_id = products.vend_id` 联结两个表   
 这种联结方式叫做等值联结,也叫内部联结

- `select vend_name,prod_name,prod_price from vendors inner join products on vendors.vend_id = products.vend_id;`  
   另一种方式：inner join 内部联结 推荐用法  

-  `select prod_name,vend_name,prod_price,quantity from orderitems,products,vendors where products.vend_id = vendors.vend_id and orderitems.prod_id = products.prod_id and order_num = 20005;`  
  三个表联结  
  性能： 多表联结非常的耗资源，联结的表越多性能下降越快  

##第16章 创建高级联结
####表别名
- `select cust_name,cust_contact from customers as c, orders as o,orderitems as oi where c.cust_id = o.cust_id and oi.order_num = o.order_num and prod_id = 'TNT2';`    
  对表名起别名，可以缩短sql，注意表别名不能返回到客户机 

####不同类型的联结
- 自联结  
   <pre><code>
   select p1.prod_id,p1.prod_name 
   from products as p1,products as p2 
   where p1.vend_id = p2.vend_id and p2.prod_id = 'DTNTR';
   </code></pre>
  自联结就是两个表是相同的表，省去子查询  
  自联结比子查询性能更好  

- 自然联结  
  <pre><code>
  select c.*,o.order_num,o.order_date,oi.prod_id,
          oi.quantity,oi.item_price 
  from customers as c,orders as o,orderitems as oi 
  where c.cust_id = o.cust_id 
        and oi.order_num = o.order_num 
        and prod_id='FB';
</code></pre>

自然联结会使每个列只返回一次，这里使用c.*来选customers表的全部列  

- 外部联结  
  <pre><code>
   select customers.cust_id,orders.order_num
  from customers 
  LEFT OUTER JOIN orders on customers.cust_id = orders.cust_id;
  </code></pre>

  外部联结是为了包含那些相关表中没有关联行的行，这里使用 OUTER JOIN 来指定联结的类型  

  使用OUTER JOIN必须指定LEFT 或者 RIGHT   
  LEFT表示OUTER JOIN左边的表，RIGHT表示OUTER JOIN右边的表  
  
  如果需要从右边的表选择所有行，需要使用RIGHT OUTER JOIN： 
  <pre><code>
  select customers.cust_id,orders.order_num 
  from customers 
  RIGHT OUTER JOIN orders on customers.cust_id = orders.cust_id;
  </code></pre>

####使用带聚集函数的联结

 <pre><code>
  select customers.cust_name,customers.cust_id,count(orders.order_num) as num_ord
  from customers 
  inner join orders on customers.cust_id = orders.cust_id 
  group by customers.cust_id;
 </code></pre>

聚集函数可以喝联结一起使用  

使用联结和联结条件：
- 主要所使用的联结类型 一般使用内部联结，外部联结也有效
- 保证使用正确的联结条件
- 应该总是提供联结条件
- 一个联结中可以包含多个表，但是在测试的时候可以先分别测试联结  

##第17章  组合查询
####组合查询
两种情况需要使用组合查询：
- 在单个查询中从不同的表返回类似结构的数据
- 对单个表执行多个查询，按单个查询返回数据 
 <pre><code>
  select vend_id,prod_id,prod_price 
  from products 
  where prod_price &lt;= 5 
  union 
  select vend_id,prod_id,prod_price 
  from products 
  where vend_id in(1001,1002);
 </code></pre>

union就是执行两条select，然后合并到一个单个查询结果集中，这种合并是两条where子句的or形式  

union规则：
- union必须由2条及2条以上的select语句组成
- union的每个查询必须包含相同的列，表达式或者聚集函数
- 列数据类型必须兼容

如果两个select语句查询的结果有相同的行，union会自动去除重复的行，如果要返回全部行，需要使用union all 

 <pre><code>
select vend_id,prod_id,prod_price 
from products 
where prod_price &lt;= 5 
union all
select vend_id,prod_id,prod_price 
from products 
where vend_id in(1001,1002);
 </code></pre>

对union的结果进行排序：

 <pre><code>
 select vend_id,prod_id,prod_price 
 from products 
 where prod_price &lt;= 5 
 union 
 select vend_id,prod_id,prod_price 
 from products
 where vend_id in(1001,1002) 
 order by vend_id,prod_price;
 </code></pre>

##第18章 全文本搜索
####全文本搜索
并不是所有引擎都支持全文搜索  
Mysql最经常使用的引擎MyISAM和InnoDB中，MyISAM支持全文本搜索，而InnoDB不支持  
在给出的样例表中，专门有一个表使用了MyISAM引擎  

    mysql> show table status;
    +--------------+--------+-
    | Name         | Engine |
    +--------------+--------+-
    | customers    | InnoDB |
    | orderitems   | InnoDB |
    | orders       | InnoDB |
    | productnotes | MyISAM |
    | products     | InnoDB |
    | vendors      | InnoDB |
    +--------------+--------+-

可以看到productnotes表使用了MyISAM引擎  

 <pre><code>
   select note_text
   from productnotes 
   where match(note_text) against('rabbit');
 </code></pre>

match检索单个列note_text,搜索字段为rabbit  

这条语句等价于使用like:

 <pre><code>
  select note_text 
  from productnotes 
  where note_text like '%rabbit%';
 </code></pre>

使用match和against来使所有行都被返回：

 <pre><code>
   select note_text, match(note_text) against('rabbit') as rank 
   from productnotes;
 </code></pre>

布尔文本搜索:
 <pre><code>
  select note_text 
  from productnotes 
  where match(note_text) against('heavy' in boolean mode);
 </code></pre>

此处全文本搜索检索包含词heavy的所有行  


##第19章 插入数据
####数据插入

插入的方式：
- 插入完整的行
- 插入行的一部分
- 插入多行
- 插入某些查询结果

插入完整的行：
  <pre><code>
  insert into customers 
  values 
  (NULL,'Pep E. LaPew','100 Main Street','Los Angeles','CA',
  '90046','USA',NULL,NULL);
  </code></pre>

这里并没有指定表的列名  

 <pre><code>
 insert into customers(cust_name,
 cust_address,
 cust_city,
 cust_state,
 cust_zip,
 cust_country,
 cust_contact,
 cust_email) 
 values ('Pep E. LaPew',
 '100 Main Street',
 'Los Angeles',
 'CA','90046',
 'USA',
 NULL,
 NULL)
 </code></pre>

在给出表的列名之后，表中的列名和values中的值的次序必须对应，数目必须正确  
使用insert语句时，建议使用表的列名  

在上面的insert中，明确的列名省略了cust_id，省略的列要满足的条件：
- 该列定义为允许NULL值
- 该表定义中给出了默认的值
 如果对表中不允许NULL，而且没有默认值，mysql会插入不成功  

提高整体性能：
insert 操作可能会很耗时，而且它可能降低等待处理的select语句的性能  
我们可以通过在insert和into之间添加low_priority，来指示mysql降低insert语句的优先级  
`inset low_priority into`  

插入多行：
 <pre><code>
 insert into customers(cust_name,
 cust_address,cust_city,cust_state,cust_zip,cust_country) 
 values
 ('Pep E. LaPew','100 Main Street','Los Angeles','CA','90046','USA'),
 ('M Martian','42 Galaxy way','New York','NY','11213','USA');  
 </code></pre>

连续插入多行，用圆括号括起来，逗号隔开  


插入检索出来的数据：  
insert还可以将一条select语句的结果插入到表中  
 <pre><code>
 insert into 
 customers(cust_id,cust_contact,cust_email,cust_name,
 cust_address,cust_city,cust_state,cust_zip,cust_country)
 select cust_id, cust_contact,cust_email,cust_name,
 cust_address,cust_city,cust_state,cust_zip,cust_country 
 from custnew;
 </code></pre>
insert中的select可以使用和insert不一样的列名  

##第20章 更新和删除数据
####更新数据
更新表中的数据，使用update有两种方式：
- 更新表中特定行
- 更新表中所有行

使用update语句时不要省略where子句，不然会更新表的所有行

update语句的组成：
- 要更新的表
- 列名和他们的新值
- 确定要更新行的过滤条件

更新单列：
 <pre><code>
  update customers 
  set cust_email ='elmer@fudd.com' 
  where cust_id = 10005; 
 </code></pre>

更新多列：
 <pre><code>
 update customers 
 set cust_name = 'The Fudds',
     cust_email ='elmer@fudd.com'
 where cust_id = 10005;
 </code></pre>

如果update在更新多行时，有一行或者多行出错，整个update都会被取消，如果要发生错误也继续更新，可以使用ignore  

`update ignore customers`  

####删除数据
删除一个表中的数据，使用delete有两种方式:
- 从表中删除特定的行
- 从表中删除所有行

使用delete语句时不要省略where子句，不然会删除表的所有行


删除单行:
 <pre><code>
  delete 
  from customers 
  where cust_id=10006;
 </code></pre>

delete语句是删除表中的行，delete不删除表本身

如果要删除表的所有行，更快的方式是使用truncate table语句  
它的本质是删除原来的表并重建一个新表  

####更新删除数据的习惯
- 除非确实要打算更新和删除每一行，不然不要使用不带where子句的语句
- 保证每个表都有主键，尽可能像where子句那样使用它
- 对update或delete语句使用where子句之前，最好先用select进行测试
- 使用强制实施引用完整性的数据库

<<[第1-10章](./Chapter1-10.md) 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　>>[第21-30章](./Chapter21-30.md)