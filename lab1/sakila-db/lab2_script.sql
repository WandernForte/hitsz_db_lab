-- update staff_list set `zip code` = '518055' where ID = '1';
-- update film_list set price = 1.99 where FID = '1';
# select price from film_list where FID='1';
-- SELECT table_name, is_updatable FROM information_schema.views WHERE table_schema = 'sakila';
-- select user from mysql.user;-- 
-- DROP USER IF EXISTS 'sakila_user'@'localhost';
-- create user 'sakila_user'@'localhost' identified by '123456';
-- grant all privileges on sakila.* to 'sakila_user'@'localhost';
-- show grants for 'sakila_user'@'localhost';

# 建立一个库存和商店的视图 inventory_info

-- DROP VIEW IF EXISTS inventory_info;# -- 支持多次运行 -- 
-- CREATE VIEW inventory_info 
-- AS
-- SELECT st.store_id AS SID, inv.inventory_id AS INVID, f.title as FilmTitle, inv.last_update as Lastupdate
-- FROM store AS st JOIN inventory AS inv ON st.store_id = inv.store_id JOIN film AS f ON inv.film_id = f.film_id;

select * from inventory_info where INVID=4;

-- CREATE TRIGGER inventory_update BEFORE UPDATE ON inventory
-- 	FOR EACH ROW SET NEW.last_update = NOW();

UPDATE inventory SET store_id='2' Where inventory_id=4;
select * from inventory_info where INVID=4;