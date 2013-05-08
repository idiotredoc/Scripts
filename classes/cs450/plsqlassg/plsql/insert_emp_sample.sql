prompt this file is an example showing how to use the ins_emp_summary procedure
prompt from a PL/SQL program.  
prompt THIS FILE MUST BE RUN FROM the 'SQL>' prompt
prompt using the START command.  It will not work in the web interface
prompt This script uses SQLPLUS 'substitution variables' so that you can provide
prompt various first and last names, user names and insert_numbers.
prompt Substitution variables are the ones with ampersands in front of them.
prompt DO NOT USE SUBSTITUTION VARIABLES IN YOUR OWN PROGRAMS
prompt DO NOT USE SUBSTITUTION VARIABLES IN YOUR OWN PROGRAMS
prompt DO NOT USE SUBSTITUTION VARIABLES IN YOUR OWN PROGRAMS
pause tap enter to continue
declare
  v_fname varchar2(15) := '&first_name';
  v_lname varchar2(15) := '&last_name';
  v_ssn char(9) := '123456789';
  v_dname varchar2(12) := 'Marketing';
  v_sup_name varchar2(25) := 'James Q Borg';
  v_salary number(8,2) := 28000.00;
  v_n_deps number(2) :=2;
  v_d_proj_num number(2) := 1;
  v_d_proj_hrs number(5,2) := 15;
  v_d_proj_cost number(7,2) := v_d_proj_hrs * v_salary / 2000;
  v_nd_proj_num number(2) := 3;
  v_nd_proj_hrs number(5,2) := 30.50;
  v_nd_proj_cost number(7,2) := v_nd_proj_hrs * v_salary / 2000;
  v_user_name varchar2(10) := '&user_name';
  v_insert_number number(4):= &insert_seq_nmber;
begin
  ibl.ins_emp_summary(
       v_fname,
       v_lname,
       v_ssn,
       v_dname,
       v_sup_name,
       v_salary,
       v_n_deps,
       v_d_proj_num,
       v_d_proj_hrs,
       v_d_proj_cost,
       v_nd_proj_num,
       v_nd_proj_hrs,
       v_nd_proj_cost,
       v_user_name,
       v_insert_number);
end;
/
prompt the following code shows how to see what you have inserted into
prompt the table in the IBL account:
prompt select * from TABLE(ibl.v_emp_summary('&User_Name'));;
pause  Tap ENTER to proceed
select * from TABLE(ibl.v_emp_summary('&User_Name'));
prompt to remove rows from the table use the following code
prompt exec ibl.clean_emp_summary('XXXXX');;
prompt where XXXXX is your user id.
