****************Default name, hotkeys and toolbar resources***************************

RESOURCE TBUF APP_DEFNAME { buf="Program"; }
(Second resource in file).
Default file name - should be same as caption name in AIF file.

RESOURCE HOTKEYS R_TXED_HOTKEYS
All the menu hotkeys (shortcut keys) for this app. Where there are two lists,
the first contains unshifted keys, the second shifted keys. Translators should
rearrange the items within each list into alphabetical order of the hotkeys in
the target language - this will facilitate checking for any accidental
duplicates. (Note that, within each list, every line must end with a comma,
except for the last line which must not!)
                             
RESOURCE TOOLBAR R_TXED_CBA
Only things to translate here are what is contained within txt="" quotes.

****************Menu resources*******************************************                          

RESOURCE MENU_BAR r_txed_menubar

RESOURCE MENU_PANE r_txed_file_menu

RESOURCE MENU_PANE r_txed_save_menu
Cascade menu - parent is r_txed_file_menu

RESOURCE MENU_PANE r_txed_printoptions_menu
Cascade menu - parent is r_txed_file_menu

RESOURCE MENU_PANE r_txed_fileoptions_menu
Cascade menu - parent is r_txed_file_menu

RESOURCE MENU_PANE r_txed_edit_menu

RESOURCE MENU_PANE r_txed_find_cascade
Cascade menu - parent is r_txed_edit_menu

RESOURCE MENU_PANE r_txed_view_menu

RESOURCE MENU_PANE r_txed_zoom_menu
Cascade menu - parent is r_txed_view_menu

RESOURCE MENU_PANE r_txed_scroll_bar_cascade
Cascade menu - parent is r_txed_view_menu

RESOURCE MENU_PANE r_txed_format_menu

RESOURCE MENU_PANE r_txed_tools_menu

RESOURCE MENU_PANE r_txed_irda_cascade
Cascade menu - parent is r_txed_tools_menu

****************Dialog resources*******************************************                                           
RESOURCE DIALOG r_txed_dialog_tab
Dialog used to set tab stops and automatic indenting

RESOURCE DIALOG r_txed_dialog_setpreferences

RESOURCE DIALOG r_txed_dialog_translate
Dialog displayed when translating or finding a runtime error (different title used)

RESOURCE DIALOG r_txed_dialog_create_standard_files
Dialog used to select a folder into which files are copied from ROM

****************Infomessage string resources*******************************************                                                                              
RESOURCE TBUF r_txed_tbuf_no_procs
Displayed when 'Go to' is used and there are no PROCs defined in the file
RESOURCE TBUF r_txed_no_error_to_show
Displayed when 'Show last error' is used and there has not yet been an error
RESOURCE TBUF r_txed_not_while_running
Displayed when an attempt is made to used a menu command that is not supported while running a program
RESOURCE TBUF r_txed_file_is_read_only
Displayed when a read-only file is opened
RESOURCE TBUF r_txed_file_unchanged
Displayed when trying to save (or revert to saved) an unchanged file
RESOURCE TBUF r_txed_files_created
Displayed after the standard files have been copied from ROM, see r_txed_dialog_create_standard_files above

****************Dialog title string resources*****************************************  
RESOURCE TBUF r_txed_import_dialog_show_text
Import text dialog 'Show files' toggle button text
RESOURCE TBUF r_txed_export_dialog_title
Export as text dialog title
RESOURCE TBUF r_txed_translate_complete
Used in the dialog displayed after successful translation
RESOURCE TBUF r_txed_run_program
Dialog title used when selecting a program to run
RESOURCE TBUF r_txed_locate_error_dialog_title
Used as the title in the dialog r_txed_dialog_translate when finding a runtime error
RESOURCE TBUF r_txed_error_in_include_file
Used when translating finds an error in a file other than the current file

****************Dialog text string resources*******************************************                                                                              
RESOURCE TBUF r_txed_run_program_question
Used in the dialog displayed after successful translation
RESOURCE TBUF r_txed_open_the_file
Used when translating finds an error in a file other than the current file

****************Other string resources*************************************************
RESOURCE TBUF r_txed_running
Used as a busy message when running a program