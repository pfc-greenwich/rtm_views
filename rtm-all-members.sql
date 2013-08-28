CREATE VIEW rtm_all_members_secure AS
SELECT users.uid AS uid, 
users.name AS username,
profile_values_profile_district.value AS district, 
profile_values_profile_lastname.value AS lastname, 
profile_values_profile_firstname.value AS firstname, 
profile_values_profile_street_address.value AS street_address, 
profile_values_profile_zipcode.value AS zipcode, 
profile_values_profile_email.value AS email, 
profile_values_profile_home_telephone.value AS home_telephone, 
profile_values_profile_office_telephone.value AS office_telephone, 
profile_values_profile_mobile_telephone.value AS mobile_telephone, 
profile_values_profile_fax.value AS fax, 
profile_values_profile_occupation.value AS occupation, 
users.mail AS users_mail 
FROM rtm_users users
INNER JOIN rtm_users_roles users_roles ON users.uid = users_roles.uid 
LEFT JOIN rtm_profile_values_secure profile_values_profile_district ON users.uid = profile_values_profile_district.uid AND profile_values_profile_district.fid = '2' AND (profile_values_profile_district.private <> 1 OR profile_values_profile_district.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_lastname ON users.uid = profile_values_profile_lastname.uid AND profile_values_profile_lastname.fid = '8'  AND (profile_values_profile_lastname.private <> 1 OR profile_values_profile_lastname.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_firstname ON users.uid = profile_values_profile_firstname.uid AND profile_values_profile_firstname.fid = '9'  AND (profile_values_profile_firstname.private <> 1 OR profile_values_profile_firstname.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_street_address ON users.uid = profile_values_profile_street_address.uid AND profile_values_profile_street_address.fid = '1'  AND (profile_values_profile_street_address.private <> 1 OR profile_values_profile_street_address.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_zipcode ON users.uid = profile_values_profile_zipcode.uid AND profile_values_profile_zipcode.fid = '3'  AND (profile_values_profile_zipcode.private <> 1 OR profile_values_profile_zipcode.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_email ON users.uid = profile_values_profile_email.uid AND profile_values_profile_email.fid = '10'  AND (profile_values_profile_email.private <> 1 OR profile_values_profile_email.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_home_telephone ON users.uid = profile_values_profile_home_telephone.uid AND profile_values_profile_home_telephone.fid = '4'  AND (profile_values_profile_home_telephone.private <> 1 OR profile_values_profile_home_telephone.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_office_telephone ON users.uid = profile_values_profile_office_telephone.uid AND profile_values_profile_office_telephone.fid = '5'  AND (profile_values_profile_office_telephone.private <> 1 OR profile_values_profile_office_telephone.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_mobile_telephone ON users.uid = profile_values_profile_mobile_telephone.uid AND profile_values_profile_mobile_telephone.fid = '7'  AND (profile_values_profile_mobile_telephone.private <> 1 OR profile_values_profile_mobile_telephone.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_fax ON users.uid = profile_values_profile_fax.uid AND profile_values_profile_fax.fid = '6'  AND (profile_values_profile_fax.private <> 1 OR profile_values_profile_fax.private is null)
LEFT JOIN rtm_profile_values_secure profile_values_profile_occupation ON users.uid = profile_values_profile_occupation.uid AND profile_values_profile_occupation.fid = '11'  AND (profile_values_profile_occupation.private <> 1 OR profile_values_profile_occupation.private is null)
WHERE users_roles.rid = 3
