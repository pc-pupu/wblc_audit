<style type="text/css">
table, td, th {
	border: 1px solid #006595;
	font-size:15px;
	font-family:Times New Roman;
	height:40px;
	margin-top:20px;
}
td{
	padding-left:13px;}
th {
	background-color: #008BD1;
	color: white;
}
td:hover {
	background-color:#d4e3e5;
}
tr:nth-child(even) {background: #DBE5F0}
tr:nth-child(odd) {background: #F1F1F1}

.btn {
  background: #3498db;
  background-image: -webkit-linear-gradient(top, #3498db, #309ee3);
  background-image: -moz-linear-gradient(top, #3498db, #309ee3);
  background-image: -ms-linear-gradient(top, #3498db, #309ee3);
  background-image: -o-linear-gradient(top, #3498db, #309ee3);
  background-image: linear-gradient(to bottom, #3498db, #309ee3);
  -webkit-border-radius: 9;
  -moz-border-radius: 9;
  border-radius: 32px;
  text-shadow: 0px 0px 2px #666666;
  -webkit-box-shadow: 0px 1px 4px #666666;
  -moz-box-shadow: 0px 1px 4px #666666;
  box-shadow: 0px 1px 4px #666666;
  font-family: Georgia;
  color: #ffffff;
  font-size: 19px;
  padding: 8px 20px 10px 21px;
  text-decoration: none;
}
view-contractor-license
.btn:hover {
  cursor: pointer;
}
.red-star{
	color:#FF0000;
}
.final_submit_anchor{
	text-decoration:none !important;
}
</style>

<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr><td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>FORM-IV</strong><br/><strong>APPLICATION OF LICENSE</strong></font></td></tr>
 <tr>
    <th width="40%" style="text-align:center;font-size:17px; padding-top: 8px;" >Statistics</th>
    <th width="50%" style="text-align:center;font-size:17px; padding-top: 8px;">Values</th>
    <th width="10%" style="text-align:center;font-size:17px; padding-top: 8px;">Remarks</th>
  </tr>
  <tr>
    <td>Contractor Name </td>
    <td><?php echo $contractor_personal; ?></td>
    <td><input type="checkbox" id="name_of_contractor" class="verified_yn"  <?php //echo $e_name_ck; ?> /> </td>
  </tr>
  <table>
  