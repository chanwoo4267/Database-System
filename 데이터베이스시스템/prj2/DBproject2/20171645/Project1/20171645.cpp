#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"
#include <conio.h>

#pragma comment(lib, "libmysql.lib")
#define _CRT_SECURE_NO_WARNINGS

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";

void type_one(MYSQL* connection, MYSQL conn);
void type_two(MYSQL* connection, MYSQL conn);
void type_three(MYSQL* connection, MYSQL conn);
void type_four(MYSQL* connection, MYSQL conn);
void type_five(MYSQL* connection, MYSQL conn);
void type_six(MYSQL* connection, MYSQL conn);
void type_seven(MYSQL* connection, MYSQL conn);

int main(void) {

	// connect to SQL DBMS

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		// FILE IO
		FILE* fp;
		char buffer[1000];
		fp = fopen("20171645.txt", "r");
		if (fp == NULL) printf("file open error! \n");
		// CREATE table and INSERT tuples
		while (fgets(buffer,sizeof(buffer),fp) != NULL)
		{
			if (strcmp(buffer, "DROP TABLE package_contents\n") == 0)
			{
				break;
			}
			if (mysql_query(connection, buffer))
				printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));

			memset(buffer, 0, sizeof(buffer));
		}
		
		// main query function

		char input;
		while (1)
		{
			printf("\n------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE 1\n");
			printf("\t2. TYPE 2\n");
			printf("\t3. TYPE 3\n");
			printf("\t4. TYPE 4\n");
			printf("\t5. TYPE 5\n");
			printf("\t6. TYPE 6\n");
			printf("\t7. TYPE 7\n");
			printf("\t0. QUIT\n");
			printf("\n----------------------------------\n");
			input = _getch();
			// break function
			if (input == '0')
				break;
			else if (input == '1')
				type_one(connection, conn);
			else if (input == '2')
				type_two(connection, conn);
			else if (input == '3')
				type_three(connection, conn);
			else if (input == '4')
				type_four(connection, conn);
			else if (input == '5')
				type_five(connection, conn);
			else if (input == '6')
				type_six(connection, conn);
			else if (input == '7')
				type_seven(connection, conn);
		}
		
		// drop table and close connection
		do 
		{
			if (mysql_query(connection, buffer))
				printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));

			memset(buffer, 0, sizeof(buffer));
		} while (fgets(buffer, sizeof(buffer), fp) != NULL);

		fclose(fp);
		mysql_close(connection);
	}

	return 0;
}

void type_one(MYSQL* connection, MYSQL conn)
{
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char track_num[100];
	char buffer[1000] = "select * from customer c, (select customer_id from orders odr, (select order_id from shipment where company_name = 'USPS' and tracking_number = ";
	char temp[1000] = " and status = 'Stopped by accident') tracker where odr.order_id = tracker.order_id) cid where c.customer_id = cid.customer_id";
	int flag = 0;
	char input;

	printf("------- TYPE  1  -------\n\n");
	printf("** Assume the package shipped by USPS with tracking number X is reported to have been destroyed in an accident. Find the contact information for the customer. **\n");
	printf(" Which X? : ");
	scanf("%4s", track_num);
	fflush(stdin);
	strcat(buffer, track_num);
	strcat(buffer, temp);

	if (mysql_query(connection, buffer))
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
	else
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			flag = 1;
			printf("\ncustomer_id : %s\ncustomer_name : %s\nphone_number : %s\n\n", sql_row[0], sql_row[1], sql_row[5]);
		}
		if (flag == 0)
			printf("\nNo Query Result. Please check your tracking number input.\n");

		mysql_free_result(sql_result);
	}

	if (flag == 0)
		return;

	// subtype

	printf("---------- Subtypes in TYPE 1 ----------\n");
	printf("\t1. TYPE 1-1\n\n");
	input = _getch();
	
	if (input == '0')
		return;
	else if (input == '1')
	{
		printf("------- TYPE 1-1 -------\n\n");
		printf("** Find the contents of that shipment and create a new shipment of replacement items. **\n");

		strcpy(buffer, "select k.product_name, m.quantity from product k, (select w.product_id, w.quantity from ordered_product w, (select * from shipment where tracking_number = ");
		strcat(buffer, track_num);
		strcat(buffer, ") z where w.order_id = z.order_id) m where k.product_id = m.product_id");

		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("product_name : %s\t\tproduct_quantity : %s\n", sql_row[0], sql_row[1]);
			}

			mysql_free_result(sql_result);
		}

		// get biggest tracking number
		strcpy(buffer, "select * from shipment order by tracking_number desc");
		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			sql_result = mysql_store_result(connection);
			sql_row = mysql_fetch_row(sql_result);
			strcpy(temp, sql_row[0]);

			mysql_free_result(sql_result);
		}

		// set track_num to biggest track num + 1
		int t = atoi(temp);
		sprintf(temp, "%d", t + 1);

		strcpy(buffer, "INSERT INTO Shipment(tracking_number, company_name, order_id, status, origin_address, current_address, destination_address, start_date, primised_date, end_date) SELECT ");
		strcat(buffer, temp);
		strcat(buffer, ", s.company_name, s.order_id, 'Information', s.origin_address, s.origin_address, s.destination_address, '2022:05:30 12:00:00', '2022:06:02 12:00:00', NULL from shipment s where s.tracking_number = ");
		strcat(buffer, track_num);
		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
			printf("Succesfully added new shipment\n");
	}
}


void type_two(MYSQL* connection, MYSQL conn)
{
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char buffer[1000] = "select * from customer q, (select temp.customer_id from (select customer_id, sum(total_price) as s from orders where DATE(order_date) between '2021-01-01' and '2021-12-31' group by customer_id) temp order by temp.s desc limit 1) res where q.customer_id = res.customer_id";
	char result[10];
	char input;

	printf("------- TYPE  2  -------\n\n");
	printf("** Find the customer who has bought the most (by price) in the past year. **\n");

	if (mysql_query(connection, buffer))
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
	else
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("customer_id : %s\ncustomer_name : %s\ncontract : %s\ncred_card : %s\ndebit_card : %s\nphone_number : %s\n\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
			strcpy(result, sql_row[0]);
		}
		mysql_free_result(sql_result);
	}

	// subtype

	printf("---------- Subtypes in TYPE 2 ----------\n");
	printf("\t1. TYPE 2-1\n\n");
	input = _getch();

	if (input == '0')
		return;
	else if (input == '1')
	{
		printf("------- TYPE 2-1 -------\n\n");
		printf("** Find the product that the customer bought the most. **\n");

		strcpy(buffer, "select * from product pd, (select product_id from ordered_product op, (select order_id, customer_id from orders where orders.customer_id = ");
		strcat(buffer, result);
		strcat(buffer, ") od where op.order_id = od.order_id order by op.quantity desc limit 1) pdid where pd.product_id = pdid.product_id");

		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			printf("product that the customer above bought the most\n");
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("product_id : %s\nproduct_name : %s\nproduct_type : %s\nmanufacturer: %s\nprice : %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
			}

			mysql_free_result(sql_result);
		}
	}
}
void type_three(MYSQL* connection, MYSQL conn)
{
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char buffer[1000] = "select p.*, sum(op.quantity), p.price * sum(op.quantity) as dollar from product p, ordered_product op, orders o where DATE(o.order_date) between '2021-01-01' and '2021-12-31' and o.order_id = op.order_id and op.product_id = p.product_id group by product_id order by dollar desc";
	char input;
	char result[10];
	int count = 0;

	printf("------- TYPE  3  -------\n\n");
	printf("** Find all products sold in the past year. **\n");

	if (mysql_query(connection, buffer))
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
	else
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("product_id : %s\tproduct_name : %20s\ttotal sales($) : %s\n", sql_row[0], sql_row[1], sql_row[6]);
		}
		mysql_free_result(sql_result);
	}

	printf("\n---------- Subtypes in TYPE 3 ----------\n");
	printf("\t1. TYPE 3-1\n");
	printf("\t2. TYPE 3-2\n\n");
	input = _getch();

	if (input == '0')
		return;
	else if (input == '2')
	{
		printf("------- TYPE 3-2 -------\n\n");
		printf("**  Find the top 10%% products by dollar-amount sold. **\n");

		// get table size
		strcpy(buffer, "select count(1) from (select p.*, sum(op.quantity), p.price * sum(op.quantity) as dollar from product p, ordered_product op, orders o where DATE(o.order_date) between '2021-01-01' and '2021-12-31' and o.order_id = op.order_id and op.product_id = p.product_id group by product_id order by dollar desc) tb");
		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				strcpy(result, sql_row[0]);
			}
			mysql_free_result(sql_result);
		}

		count = atoi(result);
		count = int(count / 10);
		if (count <= 0) 
			count = 1;
		sprintf(result, "%d", count);

		strcpy(buffer, "select p.*, sum(op.quantity), p.price * sum(op.quantity) as dollar from product p, ordered_product op, orders o where DATE(o.order_date) between '2021-01-01' and '2021-12-31' and o.order_id = op.order_id and op.product_id = p.product_id group by product_id order by dollar desc limit ");
		strcat(buffer, result);

		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("product_id : %s\tproduct_name : %20s\ttotal sales($) : %s\n", sql_row[0], sql_row[1], sql_row[6]);
			}
			mysql_free_result(sql_result);
		}
	}
	else if (input == '1')
	{
		printf("------- TYPE 3-1 -------\n\n");
		printf("** Find the top k products by dollar-amount sold. **\n");
		printf(" Which K? : ");
		scanf("%s", result);
		printf("\n");

		strcpy(buffer, "select p.*, sum(op.quantity), p.price * sum(op.quantity) as dollar from product p, ordered_product op, orders o where DATE(o.order_date) between '2021-01-01' and '2021-12-31' and o.order_id = op.order_id and op.product_id = p.product_id group by product_id order by dollar desc limit ");
		strcat(buffer, result);

		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("product_id : %s\tproduct_name : %20s\ttotal sales($) : %s\n", sql_row[0], sql_row[1], sql_row[6]);
			}
			mysql_free_result(sql_result);
		}
	}
}
void type_four(MYSQL* connection, MYSQL conn)
{
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char buffer[1000] = "select p.*, sum(op.quantity) from product p, ordered_product op, orders o where DATE(o.order_date) between '2021-01-01' and '2021-12-31' and o.order_id = op.order_id and op.product_id = p.product_id group by product_id order by sum(op.quantity) desc";
	char input;
	char result[10];
	int count = 0;

	printf("------- TYPE  4  -------\n\n");
	printf("** Find all products by unit sales in the past year. **\n");

	if (mysql_query(connection, buffer))
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
	else
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("product_id : %s\tproduct_name : %20s\ttotal sales(quantity) : %s\n", sql_row[0], sql_row[1], sql_row[5]);
		}
		mysql_free_result(sql_result);
	}

	printf("\n---------- Subtypes in TYPE 4 ----------\n");
	printf("\t1. TYPE 4-1\n");
	printf("\t2. TYPE 4-2\n\n");
	input = _getch();

	if (input == '0')
		return;
	else if (input == '2')
	{
		printf("------- TYPE 4-2 -------\n\n");
		printf("**  Find the top 10%% products by unit sales. **\n");

		// get table size
		strcpy(buffer, "select count(1) from (select p.*, sum(op.quantity) from product p, ordered_product op, orders o where DATE(o.order_date) between '2021-01-01' and '2021-12-31' and o.order_id = op.order_id and op.product_id = p.product_id group by product_id order by sum(op.quantity) desc) tb");
		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				strcpy(result, sql_row[0]);
			}
			mysql_free_result(sql_result);
		}

		count = atoi(result);
		count = int(count / 10);
		if (count <= 0)
			count = 1;
		sprintf(result, "%d", count);

		strcpy(buffer, "select p.*, sum(op.quantity) from product p, ordered_product op, orders o where DATE(o.order_date) between '2021-01-01' and '2021-12-31' and o.order_id = op.order_id and op.product_id = p.product_id group by product_id order by sum(op.quantity) desc limit ");
		strcat(buffer, result);

		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("product_id : %s\tproduct_name : %20s\ttotal sales(quantity) : %s\n", sql_row[0], sql_row[1], sql_row[5]);
			}
			mysql_free_result(sql_result);
		}
	}
	else if (input == '1')
	{
		printf("------- TYPE 4-1 -------\n\n");
		printf("** Find the top k products by unit sales. **\n");
		printf(" Which K? : ");
		scanf("%s", result);
		printf("\n");

		strcpy(buffer, "select p.*, sum(op.quantity) from product p, ordered_product op, orders o where DATE(o.order_date) between '2021-01-01' and '2021-12-31' and o.order_id = op.order_id and op.product_id = p.product_id group by product_id order by sum(op.quantity) desc limit ");
		strcat(buffer, result);

		if (mysql_query(connection, buffer))
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		else
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("product_id : %s\tproduct_name : %20s\ttotal sales(quantity) : %s\n", sql_row[0], sql_row[1], sql_row[5]);
			}
			mysql_free_result(sql_result);
		}
	}
}

void type_five(MYSQL* connection, MYSQL conn)
{
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char buffer[1000] = "select p.* from product p, store_inventory si, store s where s.store_address = 'California' and s.store_id = si.store_id and si.quantity = 0 and si.product_id = p.product_id";

	printf("------- TYPE  5  -------\n\n");
	printf("** Find those products that are out-of-stock at every store in California. **\n");

	if (mysql_query(connection, buffer))
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
	else
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("product_id : %s\nproduct_name : %s\nproduct_type : %s\nmanufacturer: %s\nprice : %s\n\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
		}
		mysql_free_result(sql_result);
	}
}

void type_six(MYSQL* connection, MYSQL conn)
{
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char buffer[1000] = "select tracking_number, company_name, op.order_id, p.product_name, op.quantity from shipment sh, ordered_product op, product p where sh.primised_date < sh.end_date and sh.order_id = op.order_id and op.product_id = p.product_id group by order_id";

	printf("------- TYPE  6  -------\n\n");
	printf("** Find those packages that were not delivered within the promised time. **\n");

	if (mysql_query(connection, buffer))
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
	else
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("tracking_number of shipment : %s\tcompany_name of shipment : %s\norder_id : %s\tordered_product : %s\t\tquantity : %s\n\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
		}
		mysql_free_result(sql_result);
	}
}
void type_seven(MYSQL* connection, MYSQL conn)
{
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char buffer[1000] = "select * from customer c, orders o, ordered_product op, product p where DATE(o.order_date) between '2022-05-01' and '2022-06-01' and o.customer_id = c.customer_id and o.order_id = op.order_id and op.product_id = p.product_id order by c.customer_id";

	printf("------- TYPE  7  -------\n\n");
	printf("** Generate the bill for each customer for the past month. **\n");

	if (mysql_query(connection, buffer))
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
	else
	{
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("customer_id : %s\tcustomer_name : %s\norder_id : %s\t\ttotal_price : %s\norder_content : %20s\torder_quantity : %s\n\n", sql_row[0], sql_row[1], sql_row[6], sql_row[9], sql_row[15], sql_row[13]);
		}
		mysql_free_result(sql_result);
	}
}