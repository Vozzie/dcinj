
All Project configuration measures used for this solution.


* Did add a new platform for x64 (configuration manager)

Each time a project is add to the solution, remove the x64 platform, don't close the dialog and add it again

* Did set the "output directory" for ALL configurations to ( ALL = Win32/Debug, Win32/Release, x64/Debug, x64/Release )

$(SolutionDir)Output\$(Platform)\$(Configuration)\

* Did set the "intermediate directory" for ALL configurations to ( ALL = Win32/Debug, Win32/Release, x64/Debug, x64/Release )

$(SolutionDir)Build\$(ProjectName)\$(Platform)\$(Configuration)\

Note: Using the same intermediate directory for different project causes build errors

* Did add "assembly build rules" to the project by right clicking the project and selecting "Build Customizations" and 
  then checking "masm"

( if one doesn't have assembly build rules, the build configuration in the next step can be done manual as explained here:
http://www.vbib.be/index.php?/tutorials/article/57-mix-assembly-with-managed/ but this is translated to dutch with an auto translator )

* Did add 2 files, caves32.s and caves64.s, 
  excluded the 32 from 64 build and vica versa by right clicking the file, properties, and set "Exclude from build" to "Yes"
  configured the 32 to build under 32 and 64 under 64 by right clicking the file, properties, and set "Item type" to "Microsoft Macro Assembler"

* For debug builds, Turn off incremental linking , to get real address of functions instead of jmp redirection

* For debug/Win32 build, changed "debug information format" from "edit and continue" to "program database" to remove build warnings
 
* Exclude exports.def (in project properties) from x64 build to remove compiler warnings

----

TODO:
	Make behavior of the assembly procedures their return value between 64 and 32 bit the same.
	Finish comments
