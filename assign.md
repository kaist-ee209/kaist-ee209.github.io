---
layout: page
title: Assignments
permalink: /assignments/
---

<h1> Programming Assignments </h1>

<p>
We will do <font color="#FF0000">six</font> programming
assignments. Assignment 0 accounts for 5% of your total grade, while other
assignments account for 10%.
</p>

<ul>
<li> Assignment 0:
<a href="assignment0/index.html"> Drawing a Rectangle</a> (due: 09/09 11:55pm)
</li>
<li> Assignment 1:
<a href="assignment1/index.html"> A Word Counting Program</a> (due: 09/23 11:55pm) </li>
<li> Assignment 2:
<a href="assignment2/index.html"> String Manipulation</a> (due: 10/14 11:55pm) </li>
<li> Assignment 3:
<a href="assignment3/index.html"> Customer Management Table </a> (due: 11/04 11:55pm) </li>
<li> Assignment 4: (TBA)
<!-- <a href="assignment4/index.html"> Assembly Language Programming</a> -->
(due: 11/25 11:55pm) </li>
<li> Assignment 5: (TBA)
<!-- <a href="assignment5/index.html">Unix Shell Assignment</a>  -->
(due: 12/16 11:55pm) </li>
</ul>


<h2> Assignment Submission (Important) </h2>
<hr>

<p>
Use the KLMS to submit your
assignments. (Link will be announced at KLMS.) Your submission should be one gzipped tar file whose name is <b>YourStudentID_assign#.tar.gz</b>.

For example, if your student ID is 20221234, and it is for assignment #1,
please name the file as 20221234_assign1.tar.gz.

</p><p>
To create the .tar.gz, first move all your files to the directory (20221234_assign1).
</p>

<p>
<pre class="ui message">
mkdir 20221234_assign1
mv all_your_files 20221234_assign1
</pre>

</p><p>
Then, move to your directory 20221234

</p>
<pre class="ui message">
cd 20221234_assign1
</pre>

<p>
Create a .tar.gz file by the 'tar' command like

</p><p>
<pre class="ui message">
tar -zcf 20221234_assign1.tar.gz *
</pre>

</p><p>
Then, you'll see  20221234_assign1.tar.gz. If you want to decompress and release the files in it (in a different directory),

</p>
<pre class="ui message">
tar zxf 20221234_assign1.tar.gz
</pre>

<h2> Ethics Document (Important) </h2>
<hr>

<p>
For every assignment submission, please fill out and submit
the <b>pdf</b> version of <a href="../assignments/EthicsOath.docx">this
document</a> that pledges your honor that you did not violate any
ethics rules required by <a href="../policy">this course</a> and
KAIST.  You can either scan a printed version into a pdf file or make
the Word document into a pdf file after filling it out.

</p><p>
Please sign on the document and submit it along with your other
assignment files, or we won't grade your assignment.


</p><h2> Late Submission </h2>
<hr>
<p>
Students can use late submission (i.e. tokens) which can be late up
to <i>three days</i> without penalty for the first five programming
assignments. That is, you can apply your late submission days (within
3 days in total) spread over the first five programming
assignments. The minimum granulaity is one day: if you are 1 hour
late, that's still counted as one day late. Your token will be used
automatically, i.e., you will get panelty after three day delays.

Beyond the free late days, we will enforce this penalty rule for each
assignment. One will get
</p><ul>
<li> 95% of the full credit up to 3 hours late,
</li><li> 90% of the full credit up to 6 hours late,
</li><li> 85% of the full credit up to 12 hours late,
</li><li> 75% of the full credit up to 24 hours late,
</li><li> 50% of the full credit up to 48 hours late,
</li><li>  0% of the full credit beyond 48 hours late
</li></ul>

<p> <b>Important</b>: Note that we do not accept late submission for the last
assignment (assignment 5). Any late submission will result in zero credit for
the last assignment. Token is not available for assignment 5.
</p>

<p><b>No late submission is allowed!</b></p>

<h2> Collaboration Policy </h2>
<hr>

<p>
Please refer to <a href="../policy">the course policy page</a>.


</p><h2> Coding Style </h2>
<hr>

<p>
Good coding style will be one criterion for grading each
assignment. Please make sure your code has proper indentation and
descriptive comments. At the start of each file, please add your name,
student ID, and the description of the file. Make sure not to leak
any memory and check/handle every return value of function calls.
</p>

<h2> Assignment Grading </h2>
<hr>

<p>
Your submission will be graded on one of the Lab machines (eelabg12) for the
course. You are free to use other machines for coding and debugging,
but please make sure to compile and test your final version on the Lab
machines (eelabg12). In a rare case, library mismatch or O/S stack difference
(Solaris vs. Linux) can bypass some of your bugs, but they can
actually show up on the Lab machines while grading. In order to avoid
this last-minute surprise, please test on Lab machines before
submitting your work.
</p>
<p>We will use an automated program for scoring.
And it works similar below practice in assignments.
So you can check your functionality using diff command.</p>

<div class="ui message" style="margin-left:1em;"><pre>./samplewc209 &lt; <em>somefile</em> &gt; output1 2&gt; errors1
./wc209 &lt; <em>somefile</em> &gt; output2 2&gt; errors2
diff output1 output2
diff errors1 errors2
rm output1 errors1 output2 errors2</pre>
</div> 

<h2> Fix a small mistake </h2>
<hr>

<p>
Because we used an automatic grading system, trivial mistake in your submission
may cause huge amount of deduction in your assignment score.  For such
exceptional circumstances, you can request changes and get modified grading at
the TA's discretion.  You will get <font color="#FF0000">15% amount of
deduction</font> from your total score. For <b>assignment 0</b>, you will not
get this panelty. 
</p>

<h2> Extra point </h2>
<hr>
<p>
Some programming assignments provide extra points.  Except for Assignment 5,
the total score, including these extra points, cannot exceed the perfect score
that you can obtain from normal tasks.  With the exception, any extra points
from Assignment 5 that gained beyond the perfect score will affect your final
grade.
</p>

<br>


<script src="{{ "/vendor/moment.min.js" | relative_url }}"></script>
<script src="{{ "/vendor/ee209_assignment.js" | relative_url }}"></script>
