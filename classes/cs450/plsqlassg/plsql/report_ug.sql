--Nathan Lutz
--PLSQL/PHP Assignment
--CS 450
--Dr. Levinstein
--3-25-2012

CREATE OR REPLACE PROCEDURE report_ug AS

   -- First, find SSN and arrange in ascending alpha order
   CURSOR emp_rep IS
   select ssn, dno
   from EMPLOYEE
   ORDER BY ssn ASC;
   v_emp_rep emp_rep%rowtype;

   -- fname
   CURSOR fname IS
   select fname 
   from EMPLOYEE
   where ssn = v_emp_rep.ssn;
   v_fname fname%rowtype;

   -- lname
   CURSOR lname IS
   select lname
   from EMPLOYEE
   where ssn = v_emp_rep.ssn;
   v_lname lname%rowtype;

   -- dname
   CURSOR dname IS 
   select dname 
   from (DEPARTMENT join EMPLOYEE on dnumber=dno)
   where ssn = v_emp_rep.ssn;
   v_dname dname%rowtype;

   -- sup_name
   CURSOR sup_name IS
   select fname, minit, lname
   from employee
   where ssn = ( select superssn from employee where v_emp_rep.ssn = ssn); 
   v_sup_name sup_name%rowtype;

   --salary
   CURSOR salary IS
   select salary from EMPLOYEE
   where ssn = v_emp_rep.ssn;
   v_salary salary%rowtype;
   
   --n_deps
   CURSOR n_deps IS
   select nvl(count(Dependent_name),0) n_deps 
   from DEPENDENT
   where Essn = v_emp_rep.ssn;
   v_n_deps n_deps%rowtype;

   -- d_proj_num
   CURSOR d_proj_num IS
   select nvl(count (pno),0) d_proj_num
   from (WORKS_ON join PROJECT on pno=pnumber) 
   where Essn = v_emp_rep.ssn AND v_emp_rep.dno = dnum; 
   v_d_proj_num d_proj_num%rowtype;

   -- d_proj_hrs
   CURSOR d_proj_hrs IS
   select nvl(SUM(hours),0) d_proj_hrs
   from (WORKS_ON join PROJECT on pno=pnumber) 
   where Essn = v_emp_rep.ssn AND v_emp_rep.dno = dnum;
   v_d_proj_hrs d_proj_hrs%rowtype;

   -- d_proj_cost
   CURSOR d_proj_cost IS
   select nvl(sum(hours*salary/2000),0) d_proj_cost
   from project left join (works_on join employee on essn=ssn) on pnumber = pno
   where Essn = v_emp_rep.ssn AND v_emp_rep.dno = dnum;
   v_d_proj_cost d_proj_cost%rowtype;

   -- nd_proj_num
   CURSOR nd_proj_num IS
   select nvl(count (pno),0) nd_proj_num
   from (WORKS_ON join PROJECT on pno=pnumber)
   where Essn = v_emp_rep.ssn AND v_emp_rep.dno != dnum;
   v_nd_proj_num nd_proj_num%rowtype;

   -- nd_proj_hrs
   CURSOR nd_proj_hrs IS
   select nvl(SUM(hours),0) nd_proj_hrs
   from (WORKS_ON join PROJECT on pno=pnumber)
   where Essn = v_emp_rep.ssn AND v_emp_rep.dno != dnum;
   v_nd_proj_hrs nd_proj_hrs%rowtype;

   -- nd_proj_cost
   CURSOR nd_proj_cost IS
   select nvl(sum(hours*salary/2000),0) nd_proj_cost
   from project left join (works_on join employee on essn=ssn) on pnumber = pno
   where Essn = v_emp_rep.ssn AND v_emp_rep.dno != dnum;
   v_nd_proj_cost nd_proj_cost%rowtype;

   v_user_name varchar2(10) := 'NLUTZ';
   -- insert_number
   insert_number  NUMBER := 0; 


BEGIN
   for emprec in emp_rep loop
      --v_emp_rep.ssn := emprec.ssn;
      --v_emp_rep.dno := emprec.dno;

      open fname;
      fetch fname into v_fname;
      close fname;

      open lname;
      fetch lname into v_lname;
      close lname;

      open dname;
      fetch dname into v_dname;
      close dname;

      open sup_name;
      fetch sup_name into v_sup_name;
      close sup_name;

      open salary;
      fetch salary into v_salary;
      close salary;

      open n_deps;
      fetch n_deps into v_n_deps;
      close n_deps;

      open d_proj_num;
      fetch d_proj_num into v_d_proj_num;
      close d_proj_num;

      open d_proj_hrs;
      fetch d_proj_hrs into v_d_proj_hrs;
      close d_proj_hrs;

      open d_proj_cost;
      fetch d_proj_cost into v_d_proj_cost;
      close d_proj_cost;

      open nd_proj_num;
      fetch nd_proj_num into v_nd_proj_num;
      close nd_proj_num;

      open nd_proj_hrs;
      fetch nd_proj_hrs into v_nd_proj_hrs;
      close nd_proj_hrs;

      open nd_proj_cost;
      fetch nd_proj_cost into v_nd_proj_cost;
      close nd_proj_cost;

      insert_number := insert_number + 1;
      
      ibl.ins_emp_summary(
       v_fname.fname,
       v_lname.lname,
       emprec.ssn,
       v_dname.dname,
       v_sup_name,
       v_salary.salary,
       v_n_deps.n_deps,
       v_d_proj_num.d_proj_num,
       v_d_proj_hrs.d_proj_hrs,
       v_d_proj_cost.d_proj_cost,
       v_nd_proj_num.nd_proj_num,
       v_nd_proj_hrs.nd_proj_hrs,
       v_nd_proj_cost.nd_proj_cost, 
       v_user_name,
       insert_number);

   end loop;
END;
/
show errors
