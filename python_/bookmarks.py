#! /usr/bin/python

<<<<<<< HEAD
# http://www.google.com, 'my google website'
=======
# http://www.gogle.com, 'my google website'
>>>>>>> a35a9dcf7cc447b2d5116160b82eda0bc2cfa178
# a simple bookmarks manage tool by python.
# bookmark struct:
# 'website_name' : 'url' : 'optional commit'

<<<<<<< HEAD

# Short name of functions

=======
# Short name of functions
>>>>>>> a35a9dcf7cc447b2d5116160b82eda0bc2cfa178
ANB = 'add_new_bookmark'
MEB = 'modi_exist_bookmark'
DB  = 'delete_bookmark'
SAQ = 'save_and_quit'

import string
import os

def fun_main_menu():
    print '-' * 62
    print '|  1.) (A)dd new bookmark     2.) (M)odify existed bookmark  |'
    print '+','-' * 58,'+'
    print '|  3.) (D)elete bookmark      4.) (S)ave and (Q)uit          |'
    print '-' * 62

bookmarks = {}
def fun_subs_menu(function):
    if function == ANB:
        website_name =''
        url = '' 
        commit =''
        website_name = str (raw_input('website name:'))
        while (website_name in bookmarks.keys()): 
            website_name = str (raw_input('existed name! another:'))
        bookmarks.update({website_name : {url : commit}})

        url = str (raw_input('url:'))
        bookmarks.update({website_name : {url : commit}})
        commit = str (raw_input('favorite commit:'))
        bookmarks.update ({website_name : {url: commit}})
        #print bookmarks
    elif function == MEB:
        print 'Waht do u want to modify? [name]>> ',
        t = raw_input()
        if t != '\n':
            website_name = t
        c = 1
        while (website_name not in bookmarks.keys()):
           print 'oh, god! forget it? will to view all items. >>'
           #try:
           #    enter = raw_input().strip()[0].lower()
           #except (EOFError, KeyboardInterrupt, IndexError):
           #    enter = 'p'
           if c > 0:
               pr_bookmarks ()
               c = c - 1
           website_name = raw_input ('website_name again: ')

        will_modi_NM = website_name
        # convert [] to string. must do it.
        will_modi_URL =  ''.join (bookmarks[will_modi_NM].keys())
        for val in bookmarks[will_modi_NM]:
            will_modi_COMM = bookmarks[will_modi_NM][val]

        website_name = raw_input ('new website_name: ')
        t = raw_input ('new url: ')
        # because raw_input convert '\n' to string, ''. so it
        # should be t == '' not t == '\n'
        url =  (will_modi_URL if t == '' else t)
        t = raw_input ('new commit: ')
        commit = (will_modi_COMM if t == '' else t)
        bookmarks.update ({website_name : {url : commit}})
        del bookmarks[will_modi_NM]
        pr_bookmarks ()
    elif function == DB:
        website_name = raw_input ('what do u want to del? :')
        while website_name not in bookmarks.keys():
            website_name = raw_input ('oh no, i cann-not find it.again: ')
        del bookmarks[website_name]
    elif function == SAQ:
        save_path = ''
        dir_name  = ''
        save_path = raw_input ('where r u want to save? :  ')
        # ./ ./sarmm . .. ../ ./ ../.. ./.
        while True:
            dir_name = os.path.dirname (save_path)
            # new -> return '', and '.' and '..' will return ''
            # './' and '../' will retrn '.' and '..'
            if save_path == '.' or save_path == '..':
                save_path = save_path + '/'
            # new -> ./new  
            elif dir_name == '': 
                save_path = './' + save_path 
                break
            elif os.path.isdir (save_path) or not os.path.exists (dir_name):
                if os.path.isdir (save_path):
                    print save_path,'is dir, again ',
                if dir_name == '':
                    pass
                elif not os.path.exists (dir_name):
                    print dir_name, 'is not exists, again  ',
                else:
                    pass
                save_path = raw_input ('>>>>')
            else:
                break
        
        if not os.path.exists (save_path):
            print 'file ', save_path, ' is  not exists, create it!'
            file_fd = file (save_path, 'w+')
            file_fd.close()
        else:
            c = ''
            print 'file ', save_path, ' is exists, (A)ppend or (O)verwrite it?'
            c = raw_input ('  :  ').strip()[0].lower() 
<<<<<<< HEAD
            if  c == 'o':
                file_fd = file (save_path, 'w+')
                file_fd.truncate (0)
                file_fd.close ()
            else:
                pass  # -- !
        try:
            file_fd = file (save_path, 'w')
        except (KeyboardInterrupt, IOError), e:
            print e

        for i in bookmarks:
            val = bookmarks[i]
            for v in val:
                file_fd.writelines (['%-20s%-40s%-60s%s' % (i, v, val[v], os.linesep)])
        file_fd.close()
            
=======
            if  c == 'a' or c == 'o':
                file_fd = file (save_path, 'w+')
                if c == 'o':
                    file_fd.truncate (0)
                    file_fd.close ()
>>>>>>> a35a9dcf7cc447b2d5116160b82eda0bc2cfa178

def pr_bookmarks():
    for key in bookmarks.keys():
        val = bookmarks[key]
        for v in val:
            print '%10s' % key, '%30s' % v, '%40s' % val[v]


fun_main_menu()
while True:
    print 'Select your choice:',
    try:
        choice = raw_input().strip()[0].lower()
    except ( EOFError, KeyboardInterrupt):
        choice = 'q'
    except (IndexError):
        choice = ''
    if choice == 'a': 
        fun_subs_menu ('add_new_bookmark')
    elif choice == 'q':
        print 'ok, exit right now...'
        exit()
    elif choice == 'm':
        fun_subs_menu ('modi_exist_bookmark')
    elif choice == 'd':
        fun_subs_menu ('delete_bookmark')
        pr_bookmarks ()
    elif choice == 's':
        fun_subs_menu ('save_and_quit')
    else:
<<<<<<< HEAD
        pr_bookmarks ()
=======
        pr_bookmarks()
>>>>>>> a35a9dcf7cc447b2d5116160b82eda0bc2cfa178












