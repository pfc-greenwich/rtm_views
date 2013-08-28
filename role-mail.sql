select rtm_users.name, rtm_users.mail, profile.value 
from rtm_users 
left join rtm_profile_values profile on profile.uid=rtm_users.uid and profile.fid=10
left join rtm_users_roles role on role.uid=rtm_users.uid
where role.rid in ( 4 )
