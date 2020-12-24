#ifndef FILE
#include <stdio.h>
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

/*
 * this define determines if we are using stubs -
 * set to 0 for stubs or 1 for no stubs
 */
#define no_stubs 1

/*
 * this define determines the interface to use
 * set to 1 for original interface or 0 for changed interface
 */
#define original_interface 1

#define MAX_FRUIT 5

int int_array[] = {1,2,3};
char str[] = {"ABC"};

/*
 * list of fruit available at the greengrocers
 */
typedef enum
{
  apple=0,orange=1,grapes=2,pear=3,melon=4
} fruit;

/*
 * names of the fruit available at the greengrocers
 */
char *fruit_names[] =
{
  "Apple","Orange","Grapes","Pear","Melon"
};

/*
 * prices of the fruit available at the greengrocers
 */
double fruit_prices[MAX_FRUIT] =
{
  0.12,0.14,0.31,0.09,0.86
};

struct customer_info
{
  fruit favourite;
  fruit cheapest;
  double cash;
};

#if original_interface
double us_dollar_er = 1.5345;
#else
double euro_er = 1.098;
#endif

struct customer_info customer;

fruit const most_healthiest = melon;
fruit const least_healthiest = pear;
const fruit* health;

/******************************************************************************
 * initialise_customer
 *
 * Initialises the customer structure with the supplied values
 *
 * Output Globals :
 * customer.favourite : set to the value of parameter favourite
 * customer.cash : set to the value of parameter cash if cash > 0
 *
 * Input Parameters :
 * favourite : set to a member of the fruit enumeration e.g. apple
 * cash : initial cash for customer e.g. 1.23
 *****************************************************************************/
void initialise_customer (fruit favourite,double cash)
{
  customer.favourite = favourite;
  customer.cheapest = pear;
  if (cash > 0.0)
  {
    customer.cash = cash;
  }
} /* end of initialise_customer */

/******************************************************************************
 * cash_injection
 *
 * Increments customer.cash by the value of parameter injection
 *
 * Returns value of customer.cash
 *
 * Output Global :
 * customer.cash : will be incremented by parameter injection if injection > 0
 *
 * Input Parameter :
 * injection : amount to add to customer.cash
 *****************************************************************************/
double customer_cash_injection (double injection)
{
  if (injection > 0.0)
  {
    customer.cash += injection;
  }

  return customer.cash;
} /* end of customer_cash_injection */

/******************************************************************************
 * get_number_of_pence
 *
 * Takes a price eg 1.34 and converts it into pence (i.e. 134)
 *
 * Returns number of pence indicated by a price
 *
 * Input Parameter :
 * price : the price to convert to pence
 *****************************************************************************/
int get_number_of_pence (double price)
{
  int pence = (int) ((price * 100) + 0.1);
  return pence;
} /* end of get_number_of_pence */

/******************************************************************************
 * convert_pence
 *
 * Takes a price and converts it into another currency
 *
 * Returns the amount in the currency converted to
 *
 * Input Parameter :
 * pence : the amount to covert
 *
 * Interface Change :
 * Demonstrates removing a global variable and adding a new one
 *****************************************************************************/
double convert_pence (int pence)
{
  double converted_price;

  converted_price = ((double) pence) / 100;
#if original_interface
  converted_price *= us_dollar_er;
#else
  converted_price *= euro_er;
#endif

  return converted_price;
} /* end of convert_pence */

#if no_stubs
/******************************************************************************
 * buy_fruit
 *
 * Buys the fruit supplied in the parameter if the customer has sufficient
 * funds to do so
 *
 * Returns TRUE if the item was bought, FALSE otherwise
 *
 * Output Global :
 * customer.cash will be modified if a fruit was successfully purchased
 *
 * Input Parameter :
 * name : member of the fruit enumeration to purchase eg apple
 *
 * Input Globals :
 * customer.cash : the funds available for the customer eg 1.23
 * fruit_prices[0] ... fruit_prices[4] retain as current value
 *****************************************************************************/
int buy_fruit (fruit name)
{
  int bought = FALSE;

  if (customer.cash >= fruit_prices[name])
  {
    customer.cash -= fruit_prices[name];
    bought = TRUE;
  }
  else
  {
    printf ("Out of money\n");
  }

  return bought;
} /* end of buy_fruit */
#else
/*
 * buy_fruit no longer exists and must be stubbed -
 * the stub will be based upon this prototype for buy_fruit
 */
int buy_fruit (fruit name);
#endif

#if no_stubs
/******************************************************************************
 * char_to_fruit
 *
 * Converts an input character into a fruit
 *****************************************************************************/
int char_to_fruit (char first_char,fruit *name)
{
  int fruit_found = TRUE;

  switch (first_char)
  {
    case 'A': case 'a':
      *name = apple;
      break;
    case 'O': case 'o':
      *name = orange;
      break;
    case 'G': case 'g':
      *name = grapes;
      break;
    case 'P': case 'p':
      *name = pear;
      break;
    case 'M': case 'm':
      *name = melon;
      break;
    default:
      fruit_found = FALSE;
      break;
  }

  return fruit_found;
} /* end of char_to_fruit */
#else
/*
 * char_to_fruit no longer exists and must be stubbed -
 * the stub will be based upon this prototype for char_to_fruit
 */
int char_to_fruit (char first_char,fruit *name);
#endif

/******************************************************************************
 * buy_fruit_ex
 *
 * Buys the fruit supplied in the parameter if the customer has sufficient
 * funds to do so. The fruit is specified by supplying the first character
 * of the fruit name
 *
 * Returns TRUE if the item was bought, FALSE otherwise
 *
 * Output Global :
 * customer.cash will be modified if a fruit was successfully purchased
 *
 * Input Parameter :
 * first_char : indicates which fruit to be e.g. 'a' will buy an apple
 *
 * Input Globals :
 * customer.cash : the funds available for the customer eg 1.23
 * fruit_prices[0] ... fruit_prices[4] retain as current value
 *****************************************************************************/
int buy_fruit_ex (char first_char)
{
  int bought = FALSE;
  int fruit_found = TRUE;
  fruit name;

  fruit_found = char_to_fruit (first_char,&name);

  if (fruit_found)
  {
    bought = buy_fruit (name);
  }

  return bought;
} /* end of buy_fruit_ex */

#if original_interface
/******************************************************************************
 * change_fruit_price
 *
 * Changes the price of the specified fruit. This can be both incremental
 * and decremental. If decrementing, the price must always be greater than 0.
 *
 * Returns the new price of the specified fruit
 *
 * Output Global :
 * fruit_prices[] : the price of the requested fruit may be changed
 *
 * Input Parameter :
 * name : member of the fruit enumeration to purchase eg apple
 * increment : the amount to increment or decrement the fruit eg 0.02
 *
 * Input Globals :
 * fruit_prices[0] ... fruit_prices[4] retain as current value
 *****************************************************************************/
double change_fruit_price (fruit name,double increment)
{
  if (fruit_prices[name] + increment > 0.0)
  {
    fruit_prices[name] += increment;
  }
  else
  {
    printf ("Price was not changed\n");
  }

  return fruit_prices[name];
} /* end of change_fruit_price */
#endif

/******************************************************************************
 * calculate_cheapest_fruit
 *
 * Returns the cheapest item of fruit available in the greengrocers
 *
 * Input Globals :
 * fruit_prices[0] ... fruit_prices[4] retain as current value
 *****************************************************************************/
fruit calculate_cheapest_fruit ()
{
  fruit cheapest;
  int i;
  double cheapest_price = 1000000.00;

  for (i=0;i<MAX_FRUIT;i++)
  {
    if (fruit_prices[i] < cheapest_price)
    {
      cheapest_price = fruit_prices[i];
      cheapest = (fruit)i;
    }
  }

  return cheapest;
} /* end of calculate_cheapest_fruit */

/******************************************************************************
 * fruit_cheaper_than
 *
 * Returns  TRUE if the proce of the fruit is less than the supplied price
 * (or 10 pence if the interface has changed)
 *
 * Input Parameters :
 * name : the fruit to check if it is cheaper than the given price
 * price : the price to check against
 *
 * Input Globals :
 * fruit_prices[0] ... fruit_prices[4] retain as current value
 *
 * Interface Change :
 * Demonstrates the effect of removing a parameter from a procedure
 *****************************************************************************/
#if original_interface
int fruit_cheaper_than (fruit name,double price)
#else
int fruit_cheaper_than (fruit name)
#endif
{
  int cheaper;

#if original_interface
  if (fruit_prices[name] < price)
#else
  if (fruit_prices[name] < 0.10)
#endif
  {
    cheaper = TRUE;
  }
  else
  {
    cheaper = FALSE;
  }

  return cheaper;
} /* end of fruit_cheaper_than */

/******************************************************************************
 * get_cheapest_fruit
 *
 * Returns the cheapest item of fruit available in the greengrocers
 *
 * Input Globals :
 * fruit_prices[0] ... fruit_prices[4] retain as current value
 *****************************************************************************/
fruit get_cheapest_fruit ()
{
#if original_interface
  fruit cheapest;

  cheapest = calculate_cheapest_fruit ();

  customer.cheapest = cheapest;

  return cheapest;
#else
  return customer.cheapest;
#endif
} /* end_of_get_cheapest_fruit */

/******************************************************************************
 * get_favourite_fruit_price
 *
 * Returns the price of the customer's favourite fruit
 *
 * Input Parameter :
 * buyer : pointer to customer_info structure e.g. &customer
 * Note : this structure should be initialised before calling this procedure
 *
 * Input Globals :
 * fruit_prices[0] ... fruit_prices[4] retain as current value
 *
 * Interface Change :
 * Demonstrates the effect of changing the input parameter declaration type
 *****************************************************************************/
#if original_interface
double get_favourite_fruit_price (struct customer_info *buyer)
#else
double get_favourite_fruit_price (fruit fav)
#endif
{
#if original_interface
  return fruit_prices[buyer->favourite];
#else
  return fruit_prices[fav];
#endif
} /* end of get_favourite_fruit_price */

/******************************************************************************
 * how_many_favourite_fruit
 *
 * Returns the maxinum number of a customer's favourite fruit that a customer
 * can buy.
 *
 * Input Globals :
 * customer.favourite : set to a member of the fruit enumeration e.g. apple
 * customer.cash : set to an initial amount of money for the customer e.g. 1.23
 * fruit_prices[0] ... fruit_prices[4] retain as current value
 *
 * Interface Change :
 * Demonstrates the effect of adding an additional parameter
 *****************************************************************************/
#if original_interface
int how_many_favourite_fruit ()
#else
int how_many_favourite_fruit (int max_amount)
#endif
{
  double favourite_price = fruit_prices[customer.favourite];
  double available_cash = customer.cash;
  int how_many = 0;

#if original_interface
  while (available_cash >= favourite_price)
#else
  while (available_cash >= favourite_price && how_many < max_amount)
#endif
  {
    how_many++;
    available_cash -= favourite_price;
  }

  return how_many;
} /* end of how_many_favourite_fruit */

/******************************************************************************
 * write_price_list
 *
 * Writes to a file a list of all fruit available in the greengrocers and their
 * respective prices. The combined price of all the items is returned.
 *
 * Input Parameters :
 * fp : set to fopen ("prices.txt","w")
 * Demonstrates assigning an input parameter to a return from a procedure call
 *
 * Input Globals :
 * fruit_names[0] ... fruit_names[MAX_FRUIT-1]
 * fruit_prices[0] ... fruit_prices[MAX_FRUIT-1]
 * All input globals should retain their current value
 *
 * Interface Change :
 * Demonstrates the effect of changing the return type
 *****************************************************************************/
#if original_interface
double write_price_list (FILE *fp)
#else
int write_price_list (FILE *fp)
#endif
{
  int i;
#if original_interface
  double total_price = 0;
#else
  int total_price = 0;
#endif

  fprintf (fp,"Greengrocers Price List\n\n");
  fprintf (fp,"FRUIT      PRICE(£)\n");
  for (i=0;i<MAX_FRUIT;i++)
  {
    fprintf (fp,"%-10s %.2f\n",
             fruit_names[i],fruit_prices[i]);
#if original_interface
    total_price += fruit_prices[i];
#else
    total_price += get_number_of_pence (fruit_prices[i]);
#endif
  }

  fclose (fp);

  return total_price;
} /* end of write_price_list */

/******************************************************************************
 * buy_healthiest
 *
 * Buys upto the amount of the most or least healthiest fruit specified by the
 * input parameters. The healthiest fruit is specified in a constant.
 *
 * Returns the difference between the amount requested for purchase and the
 * number actually bought
 *
 * Input Parameters :
 * amount : number of healthiest fruit to buy depending on sufficient funds
 * most : if TRUE then buy the lealthies fruit - otherwise least healthiest
 *
 * Input Globals :
 * healthiest : constant
 *****************************************************************************/
int buy_healthiest (const int amount,const int most)
{
  int i;
  int ok = TRUE;
  int count = 0;

  if (most)
  {
    health = &most_healthiest;
  }
  else
  {
    health = &least_healthiest;
  }

  for (i=0;i<amount;i++)
  {
    if (ok)
    {
      ok = buy_fruit (*health);
      if (ok)
      {
        count++;
      }
    }
  }

  return amount - count;
} /* end of buy_healthiest */

/******************************************************************************
 *
 * main routine
 *
 *****************************************************************************/
int main ()
{
  int num,i;
  fruit cheapest;
  double currency;

  write_price_list (stdout);
  printf ("\n");

  initialise_customer (apple,1.99);
#if original_interface
  num = how_many_favourite_fruit ();
#else
  num = how_many_favourite_fruit (1.50);
#endif
  for (i=0;i<num;i++)
  {
/*
 * buy as many of the customer's favourite fruit as possible
 */
    buy_fruit (customer.favourite);
  }
  customer_cash_injection (1.36);
/*
 * buy a pear
 */
  buy_fruit_ex ('p');
  cheapest = get_cheapest_fruit ();
#if original_interface
  if (fruit_cheaper_than (cheapest,0.10))
#else
  if (fruit_cheaper_than (cheapest))
#endif
  {
#if original_interface
    printf ("The cheapest fruit is less than 10 pence\n");
    printf ("Increasing price by two pence\n");
    change_fruit_price (cheapest,0.02);
#else
    customer_cash_injection (0.10);
#endif
  }
#if original_interface
  if (customer.cash > get_favourite_fruit_price (&customer))
#else
  if (customer.cash > get_favourite_fruit_price (customer.favourite))
#endif
  {
    buy_fruit (customer.favourite);
  }
  buy_healthiest (1,TRUE);
  num = get_number_of_pence (customer.cash);
  printf ("The customer has %d pence left\n",num);
  currency = convert_pence (num);
#if original_interface
  printf ("This is %f in US dollars\n",currency);
#else
  printf ("This is %f in Euros\n",currency);
#endif
  return 0;
}

