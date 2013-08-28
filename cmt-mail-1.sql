select u.mail,p.value from rtm_content_field_cmte_member m, rtm_content_field_cmte c,rtm_users u, rtm_profile_values p where m.nid=c.nid and m.vid = c.vid and c.field_cmte_value in ('Claims','Labor Contracts') and m.field_cmte_member_uid is not null and m.field_cmte_member_uid=u.uid and m.field_cmte_member_uid=p.uid and p.fid=10;

select uid,value from rtm_profile_values where fid=2 and value=9;

select distinct m.field_cmte_member_uid from rtm_content_field_cmte_member m, rtm_content_field_cmte c where m.nid=c.nid and m.vid = c.vid and c.field_cmte_value in ('Claims','Labor Contracts') and m.field_cmte_member_uid is not null;
