BOOL WINAPI PrivilegeCheck(
  __in          HANDLE ClientToken,
  __in_out      PPRIVILEGE_SET RequiredPrivileges,
  __out         LPBOOL pfResult
);

Running with Administrator Privileges
The first step in establishing which type of account your application needs to run under is to examine what resources the application will use and what privileged APIs it will call. You may find that the application, or large parts of it, do not require administrator privileges. Writing Secure Code, by Michael Howard and David LeBlanc offers an excellent description of how to carry out this assessment and is highly recommended.

You can provide the privileges your application needs with less exposure to malicious attack by using one of the following approaches:


Run under an account with less privilege. One way to do this is to use PrivilegeCheck to determine what privileges are enabled in a token. If the available privileges are not adequate for the current operation, you can disable that code and ask the user to logon to an account with administrator privileges. 
Break into a separate application functions that require administrator permissions. You can provide for the user a shortcut that executes the RunAs command. For detailed instructions on how to set up the shortcut, search for "runas" in Help. Programmatically, you can configure the RunAs command under the AppId registry key for your application. 
Authenticate the user by calling CredUIPromptForCredentials (GUI) or CredUICmdLinePromptForCredentials (command line) to obtain user name and password. For an example, see Asking the User for Credentials. 
Impersonate the user. A process that starts under a highly privileged account like System can impersonate a user account by calling ImpersonateLoggedOnUser or similar Impersonate functions, thus reducing privilege level. However, if a call to RevertToSelf is injected into the thread, the process returns to the original System privileges. 
If you have determined that your application must run under an account with administrator privileges and that an administrator password must be stored in the software system, see Handling Passwords for methods of doing this safely.



Send comments about this topic to Microsoft 

Build date: 8/15/2007
