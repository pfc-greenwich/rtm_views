

select * from rtm_content_field_cmte_member member, rtm_content_field_cmte cmte where member.vid=cmte.vid and cmte.field_cmte_value='Appointments'


select user.mail from rtm_content_field_cmte_member member, rtm_content_field_cmte cmte, rtm_users user where member.vid=cmte.vid and cmte.field_cmte_value='Technology Advisory' and member.field_cmte_member_uid=user.uid

select rtm_users.name, rtm_users.mail, profile.value from rtm_users left join rtm_profile_values profile on profile.uid=rtm_users.uid and profile.fid=10


select rtm_users.name, rtm_users.mail, profile.value from rtm_users 
left join rtm_profile_values profile on profile.uid=rtm_users.uid and profile.fid=10
left join rtm_content_field_cmte_member member on member.field_cmte_member_uid=rtm_users.uid
left join rtm_content_field_cmte cmte on member.vid=cmte.vid
where cmte.field_cmte_value='Appointments' 
