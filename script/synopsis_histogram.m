% Read histogram from synopsis.
clear all; clc;

num_synopses = 10;
half_num_synopses = 5;
num_servers_str = '100';
batch_size = 20;
arrival_rate_str = '0.500000';
small_probe_ratio_str = '1.600000';
total_time_str = '100.000000';

num_servers = str2double(num_servers_str);
batch_size_str = num2str(batch_size);
arrival_rate = str2num(char(arrival_rate_str)).';
digits_after_decimal_point = 6;
total_time = str2num(char(total_time_str)).';
probe_ratio_str = {'2.000000', small_probe_ratio_str,...
    small_probe_ratio_str};
probe_ratio = str2num(char(probe_ratio_str)).';
policy = {'mit', 'bs', 'bswf'};
policy_name = {'Pod', 'BS', 'BF'};
color = {'bo', 'gs', 'rv'};
for I_rate = 1:length(arrival_rate)
    for I_policy = 1:length(policy)
        filename_suffix{I_rate, I_policy} = ['synopsis_n',...
            num_servers_str, '_b', batch_size_str, '_a',...
            arrival_rate_str, '_t', total_time_str, '_r',...
            probe_ratio_str{I_policy},'_', policy{I_policy}, '_',...
            probe_ratio_str{I_policy}];
        queue{I_rate, I_policy} = dlmread(['queue_',...
            filename_suffix{I_rate, I_policy}]);
        queue_last_half = queue{I_rate, I_policy}(num_synopses, :)-...
            queue{I_rate, I_policy}(half_num_synopses, :);
        max_length = length(queue_last_half)-2;
        histogram(I_policy, 1:max_length+1) = queue_last_half(2:end)...
            /queue_last_half(1)/num_servers;
    end
end
histogram = histogram.';
histogram = [ones(1, length(policy));histogram]-[histogram; zeros(1,...
    length(policy))];
figure
h = bar(0:size(histogram, 1)-1, histogram, 'grouped');
grid on
legend(h, policy_name{1}, policy_name{2}, policy_name{3});
xlabel('Queue length')
ylabel('Fraction')

filename_infix = ['n', num2str(num_servers), '_b', num2str(batch_size),...
    '_a', num2str(arrival_rate), '_r', num2str(probe_ratio(end))];
saveas(gcf, ['histogram_', filename_infix, '.fig'], 'fig')
saveas(gcf, ['histogram_', filename_infix, '.eps'], 'psc2')
