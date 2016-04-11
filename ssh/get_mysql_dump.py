# -*- coding: utf-8 -*- 
import paramiko
import time
import os

def ssh_mysql_dump(ip, port, db_name, tbl_name, dump_name, t1, t2):
	try:
		ssh = paramiko.SSHClient()
		ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
		ssh.connect(ip, port, 'username', 'PASSWORD', timeout=5)
		
		mysqldump_cmd = 'mysqldump -uroot %s %s -w "t_when > from_unixtime(%d) and t_when < from_unixtime(%d)" > %s' % (db_name, tbl_name, t1, t2, dump_name)
		print mysqldump_cmd
		ok = ssh_exec_cmd_helper(ssh, mysqldump_cmd, ip)
		if not ok:
			return
		ssh.close()
	except Exception, e:
		print ip, 'ssh_mysql_dump Error', e
	
	username = 'username'
	privatekey = os.path.expanduser('~/.ssh/id_rsa')
	mykey = paramiko.RSAKey.from_private_key_file(privatekey, password='PASSWORD')
	transport = paramiko.Transport((ip, port))
	transport.connect(username=username, pkey=mykey)

	sftp = paramiko.SFTPClient.from_transport(transport)
	sftp.get('/home/yourname/%s' % dump_name, '/home/yourname/download/mysqldump-to-csv/%s' % dump_name)
	sftp.close()
	transport.close()
	

def mysqldump_to_csv(dump_name, csv_name):
	try:
		mysqldump_to_csv_cmd = 'cd ~/download/mysqldump-to-csv/ && python mysqldump_to_csv.py %s > %s' % (dump_name, csv_name)
		os.system(mysqldump_to_csv_cmd)
	except Exception, e:
		print 'mysqldump_to_csv Error', e


def ssh_exec_cmd_helper(ssh, cmd, ip):
	stdin, stdout, stderr = ssh.exec_command(cmd)
	errors = stderr.readlines()
	if errors:
		print ip, cmd, errors
		return False
	
	return True

if __name__ == "__main__":
	ip = 'your.remote.ip.addr'
	port = 22
	db_name = 'your_db_name'
	tbl_name = 'your_tbl_name'
	dump_name = 'dumpname.db'
	csv_name = 'csvname.csv' 
	t2 = int(time.time())
	t1 = t2 - 86400 * 30
	print 'ssh mysqldump...'
	ssh_mysql_dump(ip, port, db_name, tbl_name, dump_name, t1, t2)
	print 'mysqldump to csv...'
	mysqldump_to_csv(dump_name, csv_name)
	print 'done!'
	

