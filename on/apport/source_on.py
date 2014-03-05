from apport.hookutils import *

def add_info(report, ui=None):
    attach_file_if_exists(report, '/var/log/foo.log', key='FooLog')
    attach_related_packages(report, ['onpanel', 'onshell'])
    #report['SecretMessage'] = 'my hook was here'
    if not apport.packaging.is_distro_package(report['Package'].split()[0]):
        report['ThirdParty'] = 'True'
        report['CrashDB'] = 'on'

