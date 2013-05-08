spool questions.out
set sqlprompt ''
set echo on
rem ========================================
rem
rem Author: Nathan Lutz <nlutz>
rem SQL Assignment for CS 450, Spring 2012
rem
rem
rem ========================================
rem
rem Question 1. Retrieve the first and last names and department
rem number and name of all employees directly supervised by James Borg.
rem Show results in ascending alpha order (by last name and then first
rem name).
rem Column Headings: FNAME LNAME DNUMBER DNAME
rem ========================================
rem
select fname, lname, dnumber, dname from employee, department
where superssn =
(select ssn from employee
where fname = 'James'
and lname = 'Borg')
ORDER BY lname;
rem
rem ========================================
rem
rem Question 2. Retrieve the name and number of departments which
rem have employees who do not work on at least one project. Show results
rem in ascending alpha order. (NOTE: a department should appear on this
rem list if it has an employee who does not work on any project at
rem all.)
rem Column Headings: DNAME DNUMBER
rem
rem ========================================
rem
select dname, dnumber from department 
where dnumber in(
select dno from employee e
where not exists(
select * from project p
where exists(
select * from works_on
where essn=e.ssn
and pno=p.pnumber
)
)
)
order by dname asc;
rem
rem ========================================
rem
rem Question 3. For each department, list the department name and
rem the total number of hours assigned to projects controlled by the
rem department (irrespective of the employee to whom they are assigned)
rem and the total number of hours assigned to employees of the
rem department (irrespective of the project involved). Show results in
rem ascending alpha order. 
rem Column Headings: DNAME PROJ_HRS EMP_HRS
rem
rem ========================================
rem
select dname, proj_hrs, emp_hrs 
from department, (
select dnum, sum(hours) proj_hrs
from works_on, project 
where pno=project.pnumber
group by dnum),
(select dno, sum(hours) emp_hrs
from works_on, employee, department
where essn=employee.ssn
and dno=department.dnumber
group by dno)
where dno=dnum
and dnumber=dno
ormr by dname;
rem
rem ========================================
rem
rem Question 4. Retrieve the names of departments which have at
rem least one project which employs every one of the employees of the
rem department that controls the project. Also show the name of the
rem project. Show results in ascending alpha order. 
rem Column Headings: DNAME PNAME
rem

rem
rem ========================================
rem
rem Question 5. Retrieve the first and last names of employees who
rem work on projects which are not controlled by their departments. Also
rem show the names of the projects, the employee's department number, and
rem the number of the project's controlling department. (All of this
rem should be shown in the same result table.) Show results in
rem ascending alpha order (by last name and then first name and then
rem project name). 
rem Column Headings: FNAME LNAME PNAME E_DNUM P_DNUM
rem
rem ========================================
rem
select fname,lname,pname,dno,dnum from employee, works_on, project
where essn=ssn
and dnum<>dno
and pno=pnumber
order by lname,fname,pname;
rem
rem ========================================
rem Question 6. Retrieve the first and last names of employees who
rem work on more than the average number of projects. (Note: employees
rem who do not work on any project are to be included in the average.)
rem Display their names, the number of projects they work on, and the
rem average number of projects. (The same average should be repeated in
rem each row.) Show results in ascending alpha order (by last name and
rem then first name). [The average number of projects is the
rem average number of projects worked on per employee.] 
rem Column Headings: FNAME LNAME NUM_PRJ PRJ_AVG
rem
rem ========================================
rem
select fname,lname,(select count(pno) from works_on where essn=ssn) NUM_PRJ, (select count(distinct essn)/count(distinct pno) from works_on) PRJ_AVG from employee 
where (select count(pno) from works_on where essn=ssn) >
(select count(distinct essn)/count(distinct pno) from works_on)
order by lname,fname;
rem
rem ========================================
rem
rem Question 7. Retrieve the name and number of the project which
rem uses the most employees. Also show the total number of employees for
rem that project. If there is more than one project that has attained
rem that maximum, list them all. Show results in ascending alpha order. 
rem Column Headings: PNAME PNUMBER E_TOTAL
rem
rem ========================================
rem
SELECT pname,pnumber,e_total from
(select pname, pnumber, count(essn) as e_total
from project, works_on
where pno = pnumber
group by pnumber) P
WHERE P.e_total = max(P.e_total);
rem
rem ========================================
rem 
set echo off
spool off
set sqlprompt sql>
exit

