import smtplib
import sys

fromaddr = "random@mailinator.com"
toaddrs  = ["bugs@mailinator.com","other@mailinator.com"]
smtp_host = 'mailinator.com'

lines = sys.stdin.readlines()
msg = ''.join(lines)
server = smtplib.SMTP(smtp_host)
server.set_debuglevel(1)
#server.login('user','pw')
server.sendmail(fromaddr, toaddrs, msg)
server.quit()

