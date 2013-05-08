<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<!-- vim:set filetype=php: -->

<html>
<head>
<style type="text/css">
H1,H2,H3,H4 {background: #bbdddd;}
BODY {margin-left:5%; margin-right:5%;}
PRE.query {font-size: 120%;
           font-style: italic;
           font-weight: 600;
           background: #ddbbdd;
}
.problem {background: #ddddbb;}
.indent {margin-left: 5%;}

</style>

<title>PHP Assignment</title>
</head>
<body>
<?
// error_reporting(E_ALL & ~E_DEPRECATED);
# note several things here
# comments may be pound signs as here or C++ double slashes
# HTML outside of PHP tags works like regular html
# so the tags above will be sent to the HTML page
#
# The first page to send is form.  We can tell whether to send the form
# because user will have provided values that end up in the $_POST array.
#
if (! isset($_POST['user_name'] , $_POST['password'] ,$_POST['sid'])
    || !$_POST['user_name'] || !$_POST['password'] || !$_POST['sid']) {
  ?><h2>Supply information to login to your Oracle Account</h2><br/><?
  # error in elmasri: PHP_SELF below must not be in quotes
    # note this is another way to send HTML to the page
    # this is creating a form.  the advantage is the PHP variable
    # value is embedded in the action tab.  When this is printed
    # the form will show up as something like
    # action="/~ibl/php/example.php"
  print <<<_HTML_
    <form method="post" action="$_SERVER[PHP_SELF]">
    Enter your oracle account id: <input type="text" name="user_name" id="user_name">
    <br/>
    Enter your oracle account password: <input type="password" name="password" id="password">
    <br/>
    Enter your oracle account SID: <input type="text" name="sid" id="sid">
    <br/>
    <input type='submit' value='SUBMIT INFO'>
    </form>
_HTML_;
  #error in elmasri: _HTML_ must be snug up against LHS, not indented
} else {  # we have the values we need, so we can go to work.
?>
<h1>PHP Assignment</h1>

<table border="1">
    <tr><td colspan="2">This page is the solely the work of</td></tr>
    <tr><td>Terry Stilwell</td><td>tstilwel</td></tr>
    <tr><td>Andrew Moss</td><td>amoss</td></tr>
    <tr><td colspan="2">We have not recieved aid from anyone<br>
    else in this assignment.  We have not given <br>
      anyone else aid in the 
    assignment</td></tr>
</table>
<?

  require_once 'MDB2.php';
  # elmasri uses DB.php which is now out of date
  #
  #here is the magic incantation for how to connect
   $sid = strtoupper($_POST['sid']);
//  $sid .= '.cs.odu.edu'; #new fall10--error in oracle setup?
//  not this semester
   $uid = strtoupper($_POST['user_name']);
  # to limit access to the page, uncomment the following code
  # and add or subtract UIDs from the $legal_names array
  # (be sure the 'HTML;' ends up flush with the LHS to terminate the print statement
   $legal_names=array(
     'IBL','IBLA','IBLB','IBLC','IBLD', 'TSTILWEL', 'AMOSS');
  $legal = 0;
   for($i=0; $i<count($legal_names); $i++){
    if(! strcmp($uid,$legal_names[$i]) ){
      $legal++;
       break;
     }
  }
   if (! $legal){
     print <<<HTML
       <h1>ACCESS DENIED</h1>
       </body>
       </html>
HTML;
     exit;
   }
   $dsn= array(
   'username' =>$uid,
   'password' => $_POST['password'],
   'hostspec'=>"oracle.cs.odu.edu:1521/$sid",
   'phptype'=>'oci8',
   );
  $con =& MDB2::factory($dsn, array('emulate_database' => false));
  # factory means connection will be made when needed
  # 
///////////////////////////////////////////////////
// IMPORTANT NOTE:
// if your inserts are to be credited to you
// they have to have your ORACLE ID attached to them.
// In the code below, they are going to be credited to
// whoever logged in which might be you, your partner
// or your instructor, because they are passed with the
// parameter $uid.
//
// To make sure you get credit, be sure you uncomment
// the following line and replace XXXXXXXX with your
// ORACLE ID:
//
    $uid = 'TSTILWEL';
//
// DO NOT make this change until you are ready to submit
// this code.  The insert and clean procedures will not work
// properly for anyone except your instructor and $uid
// after you make the change, i.e., they will not work for
// your partner.
// 
// Be sure not to move this line earlier in the code
// because the orginal $uid is needed to connect the
// user and their password with the DB
///////////////////////////////////////////////////
  # some error checking
  function error_check($e2check,$e_in_msg){
     global $con;
    if(PEAR::isError($e2check)) {
      echo("<br>Error in $e_in_msg : ");
      echo( $e2check->getMessage() );
      echo( ' - ');
      echo( $e2check->getUserinfo());
      $con->disconnect();
      print "</body></html>";
      exit;
    }else {
      echo("no error in $e_in_msg<br>");
    }
  }
  error_check($con,'factory');
  
  
  ?>
  <h3> Queries </h3>
    <h4> First clean table </h4>
  <?
  
  // you must call the appropriate ibl.clean_XXX_summary procedure with your oracle user ID hard coded as the parameter before you do any inserts.
  $query = "
      BEGIN 
         ibl.clean_emp_summary('$uid'); 
      END;
";

  print("cleaning with <pre class='query'>$query</pre>");
  $result =& $con->query($query);
  ?>
  <div class="indent"><b>Explanation</b>:
  <ul><li>Must call cleaning procedure before inserts</li>
  </ul></div>
  <?
  error_check($result,'cleaning');
  $result->free();
  
 //---------------------------------------Query 1----------- 
$query1=<<<QUERY
  select fname, lname, ssn, salary
  from employee emp
  order by ssn
QUERY;

print<<<HTML
<h3>Query1</h3>
<pre class="query">$query1</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>Get fname, lname, ssn, and salary column names for the report and making sure SSNs are listed in ascending order</li>
</ul></div>
HTML;
$result =& $con->query($query1);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result, 'retrieving query1');
   
  
/*  ?><p>Testing output</p>
  <br><table border='1'><?
  $header=0;
  while ($array = $result->fetchRow(MDB2_FETCHMODE_ASSOC)) {
    # MDB2_FETCHMODE_ASSOC makes row available as hash of key => value pairs
    if (!$header){ # print a row of headers
      $header=1;
      ?><TR><?
      foreach($array as $key => $field){
          print("<th>$key</th>");
      }
      ?></TR><?
    }
    ?><tr><?
    foreach ($array as  $field){
      print("<td>$field</td>");
    }
    ?></tr><?
  }
  # we do not free $result since we need the results for inserting below
  # but fetchRow() is an iterator that has run off the end of the result set
  # so we have to set it back to the beginning:

  $result->seek(0);
  ?></table><br><?*/
  
  $result->seek(0); //comment out if using test output above

 //---------------------------------------Query 2-----------
  $query2=<<<QUERY
  select dname
  from department, employee emp
  where dnumber = emp.dno
QUERY;

print<<<HTML
<h3>Query2</h3>
<pre class="query">$query2</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>Get the name of the employee's department</li>
</ul></div>
HTML;
$result2 =& $con->query($query2);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result2, 'retrieving query2');

  
 //---------------------------------------Query 3-----------
  $query3=<<<QUERY
 select emp1.fname, emp1.minit, emp1.lname
  from employee emp2 left join employee emp1
on emp1.ssn=emp2.superssn
order by emp2.ssn
QUERY;

print<<<HTML
<h3>Query3</h3>
<pre class="query">$query3</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>Get the employee data of the employee's supervisor</li>
</ul></div>
HTML;
$result3 =& $con->query($query3);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result3, 'retrieving query3');

  
 //---------------------------------------Query 4-----------
  $query4=<<<QUERY
 select count(dependent_name) deps
from employee left join dependent
on essn=ssn
group by ssn
order by ssn
QUERY;

print<<<HTML
<h3>Query4</h3>
<pre class="query">$query4</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>Count the number of dependent's the employee has</li>
</ul></div>
HTML;
$result4 =& $con->query($query4);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result4, 'retrieving query4');

  
 //---------------------------------------Query 5-----------
  $query5=<<<QUERY
select count(pname)
from (employee left join works_on
on essn=ssn) left join project
on dno=dnum and pno=pnumber
group by ssn
order by ssn
QUERY;

print<<<HTML
<h3>Query5</h3>
<pre class="query">$query5</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>Count the number of projects on which the employee works where the project is controlled by the employee's department  </li>
</ul></div>
HTML;
$result5 =& $con->query($query5);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result5, 'retrieving query5');
  
 
 //---------------------------------------Query 6-----------
  $query6=<<<QUERY
  select sum(hours) hours
from (employee left join works_on
on essn=ssn) left join project
on dno=dnum and pno=pnumber
group by ssn
order by ssn
QUERY;

print<<<HTML
<h3>Query6</h3>
<pre class="query">$query6</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>Add up the number of hours in which the employee works on projects controlled by the employee's department
</li>
</ul></div>
HTML;
$result6 =& $con->query($query6);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result6, 'retrieving query6');
  
  
   //---------------------------------------Query 7-----------
  $query7=<<<QUERY
 select count(pname)
from (employee left join works_on
on essn=ssn) left join project
on dno<>dnum and pno=pnumber
group by ssn
order by ssn
QUERY;

print<<<HTML
<h3>Query7</h3>
<pre class="query">$query7</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>Count the number of projects on which the employee works where the project is not controlled by the employee's</li>
</ul></div>
HTML;
$result7 =& $con->query($query7);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result7, 'retrieving query7');
  
  
   //---------------------------------------Query 8-----------
  $query8=<<<QUERY
  select sum(hours) hours
from (employee left join works_on
on essn=ssn) left join project
on dno<>dnum and pno=pnumber
group by ssn
order by ssn
QUERY;

print<<<HTML
<h3>Query8</h3>
<pre class="query">$query8</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>Add up the number of hours in which the employee works on projects not controlled by the employee's department
</li>
</ul></div>
HTML;
$result8 =& $con->query($query8);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result8, 'retrieving query8');
 
  
/*  //---------------------------------------Query 9-----------
  $query9=<<<QUERY
 
QUERY;

print<<<HTML
<h3>Query9 (bigquery)</h3>
<pre class="query">$query9</pre>
<div class="indent"><b>Explanation</b>:
<ul><li>This just unites all queries using the with construct</li>
</ul></div>
HTML;
$result9 =& $con->query($query9);
  # $con is n MDB2 factory.  So it will make the connection before it makes
  # the query.  Since this is a select, the result will be an array.
  error_check($result9, 'retrieving query9');*/
  
  
  
  
  
 
//----------------------------------Preparing for insert
?><h2>Preparing for insert</h2><?
  $i_num=1;
  while ($array = $result->fetchRow(MDB2_FETCHMODE_ASSOC))
  {
  $Rfname = $array['fname'];
  $Rlname = $array['lname'];
  $Rssn = $array['ssn'];
    
  $Rsalary = $array['salary'];
  //$Rn_deps = 5;
  //$Rd_proj_num = 6;
  //$Rd_proj_hrs = 7;
  $Rd_proj_cost = 8; //change with formula
  //$Rnd_proj_num = 6;
  //$Rnd_proj_hrs = 7;
  $Rnd_proj_cost = 8; //change with formula
    
  $array2 = $result2->fetchRow(MDB2_FETCHMODE_ASSOC);
  $Rdname = $array2['dname'];
  
  
  $array3 = $result3->fetchRow(MDB2_FETCHMODE_ASSOC);
  //$Rsup_name = 'test';
  $Sfirst = $array3['fname'];
  $Smiddle = $array3['minit'];
  $Slast = $array3['lname'];
  if ($Sfirst== NULL and $Smiddle==NULL and $Slast==NULL)
{
	$Rsup_name = 'NONE';
}  
else{

$Rsup_name= $Sfirst . " " . $Smiddle . " " .$Slast;
//$Rsup_name= $Smiddle;
	}
  
  $array4 = $result4->fetchRow(MDB2_FETCHMODE_ASSOC);
  $Rn_deps = $array4['deps'];  
  
  $array5 = $result5->fetchRow(MDB2_FETCHMODE_ASSOC);
  $Rd_proj_num = $array5['count(pname)']; 
  
  $array6 = $result6->fetchRow(MDB2_FETCHMODE_ASSOC);
  //$Rd_proj_hrs = 0;
  $tempDH = $array6['hours']; 
  if ($tempDH == NULL)
  {
	$Rd_proj_hrs = 0;
  }
  else
  {$Rd_proj_hrs = $tempDH;} 
  
  $array7 = $result7->fetchRow(MDB2_FETCHMODE_ASSOC);
  $Rnd_proj_num = $array7['count(pname)']; 
  
   $array8 = $result8->fetchRow(MDB2_FETCHMODE_ASSOC);
   $tempDH2 = $array6['hours']; 
  if ($tempDH2 == NULL)
  {
	$Rnd_proj_hrs = 0;
  }
  else
  {$Rnd_proj_hrs = $tempDH2;} 
  
  $Rd_proj_cost = $Rd_proj_hrs * $Rsalary / 2000;
  $Rnd_proj_cost = $Rnd_proj_hrs * $Rsalary / 2000;
  
  // CODE FOR USING INSERT GOES HERE
$query=<<<QUERY
      BEGIN
      ibl.ins_emp_summary(
          '$Rfname',
          '$Rlname',
           $Rssn,
		   '$Rdname',
		   '$Rsup_name',
           $Rsalary,
		   $Rn_deps, 
		   $Rd_proj_num ,
		   $Rd_proj_hrs ,
           $Rd_proj_cost,
           $Rnd_proj_num ,
		   $Rnd_proj_hrs ,
           $Rnd_proj_cost ,
           '$uid',
		   $i_num
          );
      END;
QUERY;
# if you write your code on windows rather than UNIX you will need to add this next line
# to avoid an error.  
$query=preg_replace('/\r/','',$query);

# The error occurs because Windows ends lines with two characters
# ascii 13 and 10 but unix only uses 10.  The PL/SQL process gets upset when it finds
# the carriage returns (ascii 13) in the code for the procedure calls (but in the
# SQL there is no problem).

  # IMPORTANT!!!
  # See IMPORTANT NOTE above about $uid.
  # if you do not you will not get credit for your inserts.
    print("Inserting with <pre class='query'>$query</pre>");
    $result100 =& $con->query($query);
    # using $result2 b/c $result holds result of $bigquery
    error_check($result100,'procedure call');
    $result100->free();
    $i_num++;
  
  }
  # important to free memory used by $result
  $result->free();
 $query="select * from TABLE(ibl.v_emp_summary('$uid'))";
  print <<<_HTML_
    <h4>Checking results of Database Procedure Inserts</h4>
    using query <pre class='query'>$query</pre>
_HTML_;
  $result =& $con->query($query);
  error_check($result,'querying v_emp_summary');

  ?><br><table border='1'><?
  $header=0;
  while ($array = $result->fetchRow(MDB2_FETCHMODE_ASSOC)) {
    if (!$header){
      $header=1;
      ?><TR><?
        foreach($array as $key => $field){
           print("<th>$key</th>");
        }
      ?></TR><?
    }
    ?><tr><?
       foreach ($array as  $field){
         print("<td>$field</td>");
       }
    ?></tr><?
  }
  # important to free memory used by $result
  $result->free();

 
  $con->disconnect();

}
?>

</body>
</html>