
# ER Diagram
![ER_diagram](https://github.com/ShriHari33/Projects/assets/105152886/543e0b55-8df6-4089-8177-5a6a910cf5bb)


# Relational Schema
![R_schema](https://github.com/ShriHari33/Projects/assets/105152886/06a3d359-d10f-4674-88ed-04f9b3d05625)


# Queries written

-- retrieve the item names that were ordered by more than one customer, with their count

select product.product_id, product.product_name, count(*) as order_count
from orders, orderdetails, product
where orders.order_id = orderdetails.order_id and product.product_id = orderdetails.product_id
group by product.product_id, product.product_name
having count(*) > 1
order by product.product_id;


-- retrieve all item that was ordered the most
select * from customer;
select * 
from orders, orderdetails, customer
where orders.order_id = orderdetails.order_id and customer.customer_id = orders.customer_id;
select * from orderdetails;



-- retrieve the customer names with the number of items they ordered

select customer.customer_id, customer.cfname, customer.clname, count(*) as order_count
from customer, orders, orderdetails
where orders.order_id = orderdetails.order_id and orders.customer_id = customer.customer_id
group by customer.cfname, customer.clname, customer.customer_id
order by customer.customer_id;



-- retrive the feedback given by Anjali Gupta

select review.order_feedback
from orders, review, customer
where orders.review_id = review.review_id and customer.customer_id = orders.customer_id and customer.cfname = 'Anjali' and customer.clname = 'Gupta';


-- retrieve the orderId with the ratings

select orders.order_id, review.order_rating
from orders, review
where orders.review_id = review.review_id;



-- retrieve the customers who have never ordered from us

select customer_id, customer.cfname
from customer
where customer.cfname not in (select customer.cfname
                              from customer, orders
                              where customer.customer_id = orders.customer_id);
                              


-- retrieve the products which got briefed as 'bad' by more than one customer

select product.product_id, product.product_name
from product
where product.product_id in (select orderdetails.product_id
                             from orders, orderdetails, review
                             where orders.order_id = orderdetails.order_id and orders.review_id = review.review_id and order_brief like 'bad%'
                             group by orderdetails.product_id
                             having count(*) > 1);


-- list all the customers who have only ordered 1 item in a single order

select cfname, clname
from customer
where customer_id in (select orders.customer_id
                      from orders, orderdetails
                      where orders.order_id = orderdetails.order_id
                      group by orders.customer_id
                      having count(*) = 1);


-- list all the feedbacks of customers, with their name

select cfname, clname, review.order_feedback
from customer, orders, review
where customer.customer_id = orders.customer_id and review.review_id = orders.review_id;


-- list all the customers who rated 'bad'
select cfname, clname
from customer
where customer_id in (select orders.customer_id
                      from orders, review
                      where review.review_id = orders.review_id and review.order_brief = 'bad');





-- PLSQL Queries

-- display all the customer information
DECLARE
  CURSOR c_customers IS
    SELECT CFname, CLname, Phone_Num
    FROM Customer;
  cust_fname varchar(20); 
  cust_lname varchar(20); 
  cust_phone varchar2(20);
BEGIN
  dbms_output.enable();
  OPEN c_customers;
  LOOP
    FETCH c_customers INTO cust_fname, cust_lname, cust_phone;
    EXIT WHEN c_customers%NOTFOUND;
    dbms_output.put_line(cust_fname || ' ' || cust_lname || ' ' || cust_phone);
  END LOOP;
  CLOSE c_customers;
END;

-- using a record below

DECLARE
  CURSOR c_customers IS
    SELECT CFname, CLname, Phone_Num
    FROM Customer;
  
  -- declare a record type
  TYPE customer_rec IS RECORD (
    fname Customer.CFname%TYPE,
    lname Customer.CLname%TYPE,
    phone Customer.Phone_Num%TYPE
  );
  
  -- declare a variable of the record type
  cust customer_rec;
BEGIN
  OPEN c_customers;
  
  -- loop through the cursor and fetch data into the record variable
  LOOP
    FETCH c_customers INTO cust;
    EXIT WHEN c_customers%NOTFOUND;
    
    -- access the data in the record variable using dot notation
    DBMS_OUTPUT.PUT_LINE(cust.fname || ' ' || cust.lname || ' ' || cust.phone);
  END LOOP;
  
  CLOSE c_customers;
END;
/

-- Retrieve all products with their names and prices

DECLARE
  prod_name product.product_name%type;
  prod_price product.product_price%type;
  CURSOR c_products IS
    SELECT Product_name, Product_price
    FROM Product;
BEGIN
  OPEN c_products;
  WHILE c_products%found LOOP
    fetch c_products into prod_name, prod_price;
    DBMS_OUTPUT.PUT_LINE(Prod_name || ' ' || Prod_price);
  END LOOP;
  CLOSE c_products;
END;


-- Retrieve the order details for all orders placed by a specific customer

DECLARE
  CURSOR c_orders IS
    SELECT *
    FROM Orders o
    NATURAL JOIN OrderDetails
    WHERE o.Customer_ID = 1;
  r_order c_orders%ROWTYPE;
BEGIN
  OPEN c_orders;
  LOOP
    FETCH c_orders INTO r_order;
    EXIT WHEN c_orders%NOTFOUND;
    DBMS_OUTPUT.PUT_LINE(r_order.Order_ID || ' ' || r_order.Order_time || ' ' || r_order.Product_ID || ' ' || r_order.Price);
  END LOOP;
  CLOSE c_orders;
END;
