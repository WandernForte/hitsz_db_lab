# select * from actor;
# select first_name, last_name from ((actor JOIN film_actor ON actor.actor_id=film_actor.actor_id) join film on film_actor.film_id=film.film_id)  where film.film_id=1;
-- INSERT INTO actor VALUES('201', 'GRACE', 'WHITE', NOW());
# UPDATE actor SET last_name = 'GREEN' Where actor_id = '201';
# select * from actor where actor_id = '201';
# DELETE FROM actor WHERE actor_id = '201';
# 以上是报告问题部分 #
# 1
#select store_id, address, district, city, country from (((store join address on store.address_id=address.address_id) join city on address.city_id=city.city_id) join country on city.country_id=country.country_id);
# 2
--  select first_name, last_name from ((actor JOIN film_actor ON actor.actor_id=film_actor.actor_id) join film on film_actor.film_id=film.film_id)  where film.title='ROCKY WAR';
# Select customer_id, sum(amount) as amount_sum From payment Group by customer_id  order by amount_sum desc;
# 3
# SELECT first_name, last_name, sum(amount)as sum_amount from  (customer join payment on customer.customer_id=payment.customer_id) group by customer.customer_id order by sum_amount desc limit 5; 
# 4
-- select filmid, filmname, sum(rentage) as sum_rentage 
--  from (
-- 	 select film.film_id as filmid, inventory.inventory_id as inventoryid, film.title as filmname,rental.rental_id as rentalid, payment.amount as rentage 
-- 			 from film, inventory, rental, payment where film.film_id=inventory.film_id and inventory.inventory_id=rental.inventory_id and rental.rental_id=payment.rental_id)as kk 
--              group by filmid order by sum_rentage desc limit 1;
# 5
--  select actorid, firstname, lastname, filmcnt 
-- 	 from(
-- 		 select actorid, firstname, lastname, count(distinct filmid) as filmcnt 
-- 			from (select actor.actor_id as actorid, actor.first_name as firstname, actor.last_name as lastname, film_actor.film_id as filmid 
-- 				 from actor, film_actor 
-- 					 where actor.actor_id=film_actor.actor_id) as kk 
-- 						 group by actorid) as kkk 
-- 							 where filmcnt>'35';
# 6 film inventory rental customer
--  select	distinct( fname), lname
-- 	 from (
-- 		 select film.title as filmname, film.film_id as filmid, inventory.inventory_id as ivid, rental.customer_id as cid, customer.first_name as fname, customer.last_name as lname 
-- 			 from film, inventory, rental, customer 
-- 				 where film.film_id=inventory.film_id and inventory.inventory_id=rental.inventory_id and rental.customer_id=customer.customer_id) as kk 
-- 					 where filmname<>'TELEGRAPH VOYAGE';
# 7
-- select first_name,last_name
-- from actor
-- where exists(
--     select actor_id  from film,film_actor
--     where  film_actor.film_id=film.film_id  and film.title = 'ELEPHANT TROJAN' and actor.actor_id=film_actor.actor_id
-- ) 
-- and exists(
--     select actor_id from film,film_actor
--     where film_actor.film_id=film.film_id  and film.title = 'SPLASH GUMP' and actor.actor_id=film_actor.actor_id
-- );
# 8
 -- select cataid, cata, cnt from(
-- 	select cataid, cata, count(distinct filmid)as cnt from(select category.name as cata, category.category_id as cataid, film_category.film_id as filmid from category, film_category where category.category_id=film_category.category_id )as newcate group by cataid) as catecount;
# 9 有哪些影片是 2 个商店都有库存的？ inventory, film, store
-- select distinct(filmname) from( 
-- select film.title as filmname, store.store_id as storeid, film.film_id as filmid from inventory, film, store where film.film_id=inventory.film_id and inventory.store_id=store.store_id) as film_store where exists(select store_id from inventory where filmid=film_id and storeid<>store_id);
# 10  查询单次租借影片时间最长的 6 位客户，列出其 first_name、last_name 和当次租借时长；
-- select fstname, lstname, deltdate from (# 在这里对客户id进行唯一性限制
-- 	select distinct(customer.customer_id) as customerid, customer.first_name as fstname, customer.last_name as lstname, datediff(rental.return_date, rental.rental_date) as deltdate 
-- 		from customer, rental 
-- 			where customer.customer_id=rental.customer_id) as newrental 
-- 				group by customerid 
-- 					order by deltdate 
-- 						desc limit 6;
# 11 在 customer 表中新增一条数据，注意 customer 表与其他表的关系
-- insert into customer(customer_id, store_id, first_name, last_name, address_id, active, create_date, last_update) select(select IFNULL(max(customer_id),0) from rental)+1, (select IFNULL(store_id,0) from store limit 1) ,'HAOER','TIANSUO',(select IFNULL(address_id,0) from address limit 1), 0, '2006-02-15 21:30:53',now();
-- select * from customer where first_name='HAOER'; 
-- # 12 update 
-- select * from customer where customer_id=(select IFNULL(max(customer_id),0) from rental);
-- update customer set last_update = now() where customer_id=(select IFNULL(max(customer_id),0) from rental);
-- select * from customer where customer_id=(select IFNULL(max(customer_id),0) from rental);
-- # 13 delete
-- delete from customer where customer_id=(select IFNULL(max(customer_id),0) from rental)+1;


